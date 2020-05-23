#!/usr/bin/env python3

from collections import defaultdict
import xml.etree.ElementTree as ET
import os

from genAllInstructions import whitelisted_opcodes


if __name__ == "__main__":
    f = open(os.path.join(os.path.dirname(
        os.path.realpath(__file__)), '../cpu/InstructionDecoder.cpp'), 'w')

    f.write("""
#include "InstructionDecoder.h"

#include "cpu/Cpu.h"
#include "cpu/InstructionFetcher.h"
#include "cpu/Immediate.h"
#include "cpu/ModRm.h"
#include "cpu/Opcode.h"
#include "cpu/insts/AllInstructions.h"
#include "cpu/operands/ImmediateOperand.h"
#include "cpu/operands/MemoryOperand.h"
#include "cpu/operands/ModRmOperand.h"
#include "cpu/operands/RegisterOperand.h"
#include "cpu/operands/RelativeOffsetOperand.h"
#include "cpu/operands/SegmentRegisterOperand.h"

using namespace emu::cpu::insts;

namespace emu::cpu {

InstructionDecoder::InstructionDecoder(Cpu& cpu) : cpu_{cpu} {}

std::unique_ptr<Instruction> InstructionDecoder::decode(
    FetchedInstruction fetchedInstruction) {
const auto& opcode = fetchedInstruction.opcode;
const auto& modrm = fetchedInstruction.modrm;
const auto& imm = fetchedInstruction.imm;

switch (opcode.po) {\n""")

    tree = ET.parse(os.path.join(os.path.dirname(
        os.path.realpath(__file__)), 'x86reference.xml'))
    root = tree.getroot()
    assert(root.tag == 'x86reference')

    data = defaultdict(lambda: {})

    po_has_Z = set()

    for pri_opcd in root.find('./one-byte'):
        assert(pri_opcd.tag == 'pri_opcd')
        po = int(pri_opcd.attrib['value'], 16)
        if po not in whitelisted_opcodes:
            continue

        data[po] = defaultdict(lambda: defaultdict(lambda: {}))

        entries = pri_opcd.findall('./entry')

        for entry in pri_opcd.findall('./entry'):
            pref = entry.find('./pref')

            opcd_ext = entry.find('./opcd_ext')
            if opcd_ext is not None:
                opcd_ext = opcd_ext.text

            proc_start = entry.find('./proc_start')
            if proc_start is None:
                proc_start = 0
            else:
                proc_start = int(proc_start.text)

            data[po][proc_start][opcd_ext]

            if entry.attrib.get('attr', '') == 'invd':
                data[po][proc_start][opcd_ext] = {
                    'type': 'invd'
                }
                continue
            syntaxs = entry.findall('./syntax')

            if len(syntaxs) > 1:
                if po in (0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0xd0, 0xd1, 0xd2, 0xd3):
                    syntaxs = [syntaxs[0]]
            assert(len(syntaxs) == 1)

            has_Z = any(
                [op.find('./a') is not None and op.find('./a').text == 'Z' for syntax in syntaxs for op in list(syntax)[1:]])
            if has_Z:
                po_has_Z.add(po)

            for syntax in syntaxs:
                #     assert(entry.find('./opcd_ext') is None)
                mnem = list(syntax)[0]
                data[po][proc_start][opcd_ext] = {
                    'type': 'normal',
                    'mnem': mnem.text,
                    'ops': [],
                }

                ops = list(syntax)[1:]
                for op in ops:
                    a = op.find('./a')
                    t = op.find('./t')

                    if a is None and 'type' in op.attrib:
                        t = op.attrib['type']

                    if a is not None:
                        assert(t is not None)
                        data[po][proc_start][opcd_ext]['ops'].append(
                            (a.text, t.text))
                    elif op.text in ('AL', 'eAX', 'rAX', 'DX', 'AH', 'CL'):
                        data[po][proc_start][opcd_ext]['ops'].append(
                            (op.text, t))
                    elif op.text == '1':
                        data[po][proc_start][opcd_ext]['ops'].append(
                            ('1', 'b'))
                    else:
                        print(f"Unknown a {a} {op.text} {op.attrib}")
                        exit(1)

    mnem_data = defaultdict(lambda: [])

    indent = "    "
    for po in sorted(data.keys()):
        f.write(f"{indent}case 0x{po:x}:\n")
        if po in po_has_Z:
            for i in range(1, 8):
                f.write(f"{indent}case 0x{po+i:x}:\n")
        indent += "  "

        for i, proc_start in enumerate(reversed(sorted(data[po].keys()))):
            if i == 0:
                f.write(f"{indent}if constexpr (PROC_CODE >= {proc_start}) {{\n")
            else:
                f.write(f"{indent}}} else if (PROC_CODE >= {proc_start}) {{\n")
            indent += "  "

            def helper(d):
                global indent
                ops_templates = []
                ops_params = []

                if d['type'] == 'invd':
                    f.write(
                        f"{indent}return std::make_unique<Illegal>(fetchedInstruction);\n")
                    indent = indent[:-2]
                    return

                for op in d['ops']:
                    if op[1] == 'v' or op[1] == 'vqp' or op[1] == 'vds' or op[1] == 'w':
                        opsize = 'uint16_t'
                        conv_op = 'static_cast<uint16_t>({})'
                    elif op[1] == 'p':
                        opsize = 'uint32_t'
                        conv_op = 'static_cast<uint32_t>({})'
                    elif op[1] == 'b':
                        opsize = 'uint8_t'
                        conv_op = 'static_cast<uint8_t>({})'
                    elif op[1] == 'bs':
                        opsize = 'uint16_t'
                        conv_op = 'fmt::internal::bit_cast<uint16_t>(static_cast<int16_t>(fmt::internal::bit_cast<int8_t>(static_cast<uint8_t>({}))))'
                    else:
                        print(f"Unknown opsize {op}")
                        exit(1)

                    if op[0] == 'Z':
                        ops_templates.append(f"RegisterOperand<{opsize}>")
                        ops_params.append(
                            f'RegisterOperand<{opsize}>(cpu_, opcode.r)')
                    elif op[0] == 'J':
                        ops_templates.append(
                            f"RelativeOffsetOperand<{opsize}>")
                        ops_params.append(
                            f"RelativeOffsetOperand<{opsize}>(cpu_, {conv_op.format('imm.imm')})")
                    elif op[0] == 'I' or op[0] == 'J' or op[0] == 'A':
                        ops_templates.append(f"ImmediateOperand<{opsize}>")
                        ops_params.append(
                            f"ImmediateOperand<{opsize}>({conv_op.format('imm.imm')})")
                    elif op[0] == 'E':
                        ops_templates.append(f"ModRmOperand<{opsize}>")
                        ops_params.append(
                            f"ModRmOperand<{opsize}>(cpu_, modrm)")
                    elif op[0] == 'G':
                        ops_templates.append(f"RegisterOperand<{opsize}>")
                        ops_params.append(
                            f"RegisterOperand<{opsize}>(cpu_, modrm.reg)")
                    elif op[0] == 'S':
                        assert(opsize == 'uint16_t')
                        ops_templates.append(f"SegmentRegisterOperand")
                        ops_params.append(
                            f"SegmentRegisterOperand(cpu_, modrm.reg)")
                    elif op[0] in ('AL', 'AH', 'eAX', 'rAX', 'DX', 'CL'):
                        ops_templates.append(f"RegisterOperand<{opsize}>")
                        ops_params.append(
                            f"RegisterOperand<{opsize}>(cpu_, GeneralPurposeRegister::{op[0][-2:]}.index())")
                    elif op[0] == '1':
                        ops_templates.append(f"ImmediateOperand<{opsize}>")
                        ops_params.append(
                            f"ImmediateOperand<{opsize}>(1)")
                    else:
                        print(f"Unknown optype {op}")
                        exit(1)

                if ops_templates:
                    ops_templates = f"<{', '.join(ops_templates)}>"
                else:
                    ops_templates = ""

                mnem_data[d['mnem'].capitalize()].append(
                    {
                        'nops': len(ops_params),
                        'templates': ops_templates
                    }
                )

                f.write(
                    f"{indent}return std::make_unique<{d['mnem'].capitalize()}{ops_templates}>({', '.join(ops_params)});\n")

            if None in data[po][proc_start]:
                assert(len(data[po][proc_start]) == 1)
                helper(data[po][proc_start][None])
            else:
                f.write(f"{indent}switch (modrm.o) {{\n")
                indent += "  "
                for o, d in data[po][proc_start].items():
                    f.write(f"{indent}case {o}:\n")
                    indent += "  "
                    helper(d)
                    indent = indent[:-2]
                indent = indent[:-2]
                f.write(f"{indent}}}\n")
                f.write(
                    f"{indent}return std::make_unique<Illegal>(fetchedInstruction);\n")

            indent = indent[:-2]
        f.write(f"{indent}}}\n")
        f.write(f"{indent}break;\n")
        indent = indent[:-2]

    f.write("""  }
    return std::make_unique<Illegal>(fetchedInstruction);
    }

    }  // namespace emu::cpu
    \n""")

    cmake_gens = []

    for mnem, mdata in mnem_data.items():
        nops = mdata[0]['nops']

        f = open(os.path.join(os.path.dirname(
            os.path.realpath(__file__)), f'../cpu/insts/gen/{mnem}.h'), 'w')
        f.write(f"""#pragma once

#include <fmt/format.h>

#include "cpu/insts/Instruction.h"

namespace emu::cpu::insts {{
{"" if nops == 0 else f"{chr(10)}template <{', '.join([f'typename Op{i}' for i in range(1, nops+1)])}>"}
class {mnem} : public Instruction {{
 public:
  {mnem}({", ".join([f"Op{i} op{i}" for i in range(1, nops+1)])}){"" if nops == 0 else f" : {', '.join([f'op{i}_{{op{i}}}' for i in range(1, nops+1)])}"} {{}}

  void execute(Cpu& cpu) override;

  std::string str() const noexcept override {{
    return fmt::format("{mnem.upper()}{"" if nops == 0 else f"{chr(9)}{', '.join(['{}'] * nops)}"}"{"".join([f", op{i}_.str()" for i in range(1, nops+1)])});
  }}

 private:{str(chr(10)).join([f"{chr(10)}  Op{i} op{i}_;" for i in range(1, nops+1)])}
}};

}}  // namespace emu::cpu::insts
""")
        f.close()

        if not os.path.exists(os.path.join(os.path.dirname(
                os.path.realpath(__file__)), f'../cpu/insts/{mnem}.cpp')):
            f = open(os.path.join(os.path.dirname(
                os.path.realpath(__file__)), f'../cpu/insts/{mnem}.cpp'), 'w')
            f.write(f"""#include "gen/{mnem}.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {{
{"" if nops == 0 else f"{chr(10)}template <{', '.join([f'typename Op{i}' for i in range(1, nops+1)])}>"}
void {mnem}{"" if nops == 0 else f"<{', '.join([f'Op{i}' for i in range(1, nops+1)])}>"}::execute(Cpu& cpu) {{
  throw new std::exception();
}}

}}  // namespace emu::cpu::insts

{f'#include "gen/{mnem}.cpp"' if mdata[0]['templates'] != '' else ""}
""")
            f.close()

        if mdata[0]['templates'] != '':
            f = open(os.path.join(os.path.dirname(
                os.path.realpath(__file__)), f'../cpu/insts/gen/{mnem}.cpp'), 'w')
            f.write(f"""#include "{mnem}.h"

#include "cpu/operands/ImmediateOperand.h"
#include "cpu/operands/MemoryOperand.h"
#include "cpu/operands/ModRmOperand.h"
#include "cpu/operands/RegisterOperand.h"
#include "cpu/operands/RelativeOffsetOperand.h"
#include "cpu/operands/SegmentRegisterOperand.h"

namespace emu::cpu::insts {{
""")
            for smdata in mdata:
                assert(smdata['nops'] == nops)
                templates = smdata['templates']
                f.write(
                    f"template class {mnem}{templates};{chr(10)}")
            f.write("""}  // namespace emu::cpu::insts
""")
            f.close()
            cmake_gens.append(f'src/cpu/insts/gen/{mnem}.cpp')

        cmake_gens.append(f'src/cpu/insts/{mnem}.cpp')
        cmake_gens.append(f'src/cpu/insts/gen/{mnem}.h')

    f = open(os.path.join(os.path.dirname(
        os.path.realpath(__file__)), f'../../cmake/gen.cmake'), 'w')
    f.write(f"""set(SOURCE
{chr(10).join(cmake_gens)}
)
""")
    f.close()
