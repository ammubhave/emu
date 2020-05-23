
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

switch (opcode.po) {
