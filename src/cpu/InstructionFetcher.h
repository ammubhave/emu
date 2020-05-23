#pragma once

#include "cpu/Immediate.h"
#include "cpu/ModRm.h"
#include "cpu/Opcode.h"

namespace emu::cpu {

class Cpu;
class InstructionFetcher;

class FetchedInstruction {
 public:
  explicit FetchedInstruction(InstructionFetcher& fetcher);

  template <auto T>
  void op(Cpu& cpu) const;

  Opcode opcode;
  ModRm modrm;
  Immediate imm;
};

class InstructionFetcher {
 public:
  InstructionFetcher(Cpu& cpu);

  template <typename T>
  T peek();
  template <typename T>
  T next();

  FetchedInstruction fetchNextInstruction();

  // template <auto& AddressingMethod, auto& OperandType>
  // op();

 private:
  Cpu& cpu_;
};

}  // namespace emu::cpu
