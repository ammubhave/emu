#include "Immediate.h"

#include <iostream>

#include "cpu/ImmediateOperand.h"
#include "cpu/InstructionFetcher.h"
#include "cpu/InstructionSet.h"
#include "cpu/ModRm.h"
#include "cpu/Opcode.h"

namespace emu::cpu {

Immediate::Immediate(InstructionFetcher fetcher, Opcode opcode, ModRm modrm) {
  const auto count = hasImmediate(opcode, modrm);

  switch (count) {
    case 0:
      imm = 0;
      break;
    case 1:
      imm = fetcher.next<uint8_t>();
      break;
    case 2:
      imm = fetcher.next<uint16_t>();
      break;
    case 4:
      imm = fetcher.next<uint32_t>();
      break;
  }
}

uint32_t Immediate::A() const { return static_cast<uint32_t>(imm); }

// uint16_t Immediate::Asel() const { return static_cast<uint16_t>(imm >> 16); }

// uint16_t Immediate::Aoff() const { return static_cast<uint16_t>(imm); }

uint16_t Immediate::v() const { return static_cast<uint16_t>(imm); };

ImmediateOperand<uint16_t> Immediate::op_v() const { return v(); };

uint16_t Immediate::bs() const {
  return (uint16_t)(int16_t)(int8_t)(uint8_t)imm;
}

ImmediateOperand<uint16_t> Immediate::op_bs() const { return bs(); };

}  // namespace emu::cpu
