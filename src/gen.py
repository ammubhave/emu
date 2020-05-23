#!/usr/bin/env python3

import xml.etree.ElementTree as ET
import os

tree = ET.parse(os.path.join(os.path.dirname(
    os.path.realpath(__file__)), 'py/x86reference.xml'))
root = tree.getroot()
assert(root.tag == 'x86reference')

f = open(os.path.join(os.path.dirname(
    os.path.realpath(__file__)), 'cpu/InstructionSet.cpp'), 'w')


def get_has_modrm_cases(opcodes, opcodesType):
    has_modrm_cases = ""
    assert(opcodes is not None)
    for pri_opcd in opcodes:
        assert(pri_opcd.tag == 'pri_opcd')
        po = int(pri_opcd.attrib['value'], 16)

        has_modrm = None

        for entry in pri_opcd.findall('./entry'):
            assert(entry.tag == 'entry')
            if entry.attrib.get('attr', '') == 'invd':
                continue
            syntaxs = entry.findall('./syntax')
            assert(len(syntaxs) > 0)
            for syntax in syntaxs:
                _has_modrm = None

                if entry.find('./opcd_ext') is not None:
                    _has_modrm = True

                ops = list(syntax)[1:]
                for op in ops:
                    a = op.find('./a')
                    if a is None:
                        continue
                    if a.text in ('C', 'D', 'E', 'ES', 'EST', 'G', 'H', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W'):
                        _has_modrm = True
                        break

                if _has_modrm is None:
                    _has_modrm = False

                if has_modrm is None:
                    has_modrm = _has_modrm
                elif has_modrm != _has_modrm and not (opcodesType == 'two' and po == 0xb8):
                    print(ET.dump(pri_opcd))

        if has_modrm and not (opcodesType == 'two' and po == 0xb8):
            has_modrm_cases += ("    " if opcodesType ==
                                'two' else "") + "    case " + hex(po) + ":\n"
    return has_modrm_cases


def get_has_immediate(opcodes, opcodesType):
    has_immediate_cases = {1: [], 2: [], 4: []}
    assert(opcodes is not None)
    for pri_opcd in opcodes:
        assert(pri_opcd.tag == 'pri_opcd')
        po = int(pri_opcd.attrib['value'], 16)

        has_immediate = None

        if opcodesType == 'one' and (po == 0xd4 or po == 0xd5):
            has_immediate = 1
        elif (opcodesType == 'one' and (po == 0xf6 or po == 0xf7)) or (opcodesType == 'two' and po == 0x3a):
            has_immediate = None
        else:
            for entry in pri_opcd.findall('./entry'):
                assert(entry.tag == 'entry')
                if entry.attrib.get('attr', '') == 'invd':
                    continue
                syntaxs = entry.findall('./syntax')
                assert(len(syntaxs) > 0)
                for syntax in syntaxs:
                    _has_immediate = None

                    ops = list(syntax)[1:]

                    has_Z = any(
                        [op.find('./a') is not None and op.find('./a').text == 'Z' for syntax in syntaxs for op in list(syntax)[1:]])

                    for op in ops:
                        a = op.find('./a')
                        if a is None:
                            continue
                        if a.text in ('A', 'I', 'J'):
                            t = op.find('./t')
                            assert(t is not None)
                            if t.text[0] == 'b':
                                _has_immediate = 1
                            elif t.text[0] == 'v' or t.text[0] == 'w':
                                _has_immediate = 2
                            elif t.text[0] == 'p':
                                _has_immediate = 4
                            else:
                                print(t.text)
                            break

                    if _has_immediate is None:
                        _has_immediate = False

                    if has_immediate is None:
                        has_immediate = _has_immediate
                    elif has_immediate != _has_immediate:
                        print(ET.dump(pri_opcd))

        if has_immediate:
            has_immediate_cases[has_immediate].append(po)
            if has_Z:
                for i in range(1, 8):
                    has_immediate_cases[has_immediate].append(po + i)

    has_immediate_str = ""
    for count in sorted(has_immediate_cases.keys()):
        if len(has_immediate_cases[count]) == 0:
            continue
        for po in has_immediate_cases[count]:
            has_immediate_str += ("    " if opcodesType ==
                                  'two' else "") + "    case " + hex(po) + ":\n"
        has_immediate_str += ("    " if opcodesType ==
                              'two' else "") + "      return " + str(count) + ";\n"
    return has_immediate_str


one_byte = root.find('./one-byte')
one_byte_has_modrm_cases = get_has_modrm_cases(one_byte, 'one')
one_byte_has_immediate_cases = get_has_immediate(one_byte, 'one')

two_byte = root.find('./two-byte')
two_byte_has_modrm_cases = get_has_modrm_cases(two_byte, 'two')
two_byte_has_immediate_cases = get_has_immediate(two_byte, 'two')


f.write(f"""#include "InstructionSet.h"

#include "cpu/ModRm.h"
#include "cpu/Opcode.h"

namespace emu::cpu {{

bool hasModRm(Opcode opcode) {{
  switch (opcode.po) {{
    case 0xf0: {{
      switch (opcode.po) {{
{two_byte_has_modrm_cases}          return true;
        case 0xb8:
          return opcode.pf[0] == 0xf3;
      }}
      return false;
    }}
{one_byte_has_modrm_cases}      return true;
  }}
  return false;
}}

size_t hasImmediate(Opcode opcode, ModRm modrm) {{
  switch (opcode.po) {{
    case 0xf0: {{
      switch (opcode.po) {{
{two_byte_has_immediate_cases}        case 0x3a:
          switch (opcode.so) {{
            case 0x0f:
            case 0x40:
            case 0x41:
              return 0;
          }}
          return 1;
      }}
      return 0;
    }}
{one_byte_has_immediate_cases}    case 0xf6:
    case 0xf7:
      return modrm.o == 0 || modrm.o == 1;
  }}
  return 0;
}}

}}  // namespace emu::cpu""")
f.close()

# instructions = {}


def gen_instructions(opcodes, opcodesType):
    decoder_cases = ""
    assert(opcodes is not None)
    for pri_opcd in opcodes:
        assert(pri_opcd.tag == 'pri_opcd')
        po = int(pri_opcd.attrib['value'], 16)

        for entry in pri_opcd.findall('./entry'):
            assert(entry.tag == 'entry')
            if entry.attrib.get('attr', '') == 'invd':
                continue
            syntaxs = entry.findall('./syntax')
            assert(len(syntaxs) > 0)
            for syntax in syntaxs:
                _has_modrm = None

                if entry.find('./opcd_ext') is not None:
                    _has_modrm = True

                ops = list(syntax)[1:]
                for op in ops:
                    a = op.find('./a')
                    if a is None:
                        continue
                    if a.text in ('C', 'D', 'E', 'ES', 'EST', 'G', 'H', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W'):
                        _has_modrm = True
                        break

                if _has_modrm is None:
                    _has_modrm = False

                if has_modrm is None:
                    has_modrm = _has_modrm
                elif has_modrm != _has_modrm and not (opcodesType == 'two' and po == 0xb8):
                    print(ET.dump(pri_opcd))

        if has_modrm and not (opcodesType == 'two' and po == 0xb8):
            has_modrm_cases += ("    " if opcodesType ==
                                'two' else "") + "    case " + hex(po) + ":\n"
    return has_modrm_cases


# f = open(os.path.join(os.path.dirname(
#     os.path.realpath(__file__)), 'cpu/insts/'), 'w')
f.close()
