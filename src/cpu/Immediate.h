#pragma once

#include <cstdint>

namespace emu::cpu {

template <typename T>
class ImmediateOperand;
class InstructionFetcher;
class ModRm;
class Opcode;

class Immediate {
 public:
  Immediate(InstructionFetcher fetcher, Opcode opcode, ModRm modrm);

  uint32_t A() const;

  // uint16_t Asel() const;

  // uint16_t Aoff() const;

  ImmediateOperand<uint16_t> op_v() const;

  uint16_t v() const;

  ImmediateOperand<uint16_t> op_bs() const;

  uint16_t bs() const;

  uint64_t imm;
};

}  // namespace emu::cpu
