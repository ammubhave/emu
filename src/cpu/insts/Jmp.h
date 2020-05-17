#pragma once

#include <cstdint>

#include "cpu/MemoryOperand.h"
#include "cpu/ModRmOperand.h"
#include "cpu/insts/Instruction.h"

namespace emu::cpu::insts {

template <bool IsNearJump, bool IsRelativeJump, bool IsDirectJump>
class JmpInstruction : public Instruction {
  using Operand = std::conditional_t<
      IsNearJump,
      std::conditional_t<IsRelativeJump, uint16_t, ModRmOperand<uint16_t>>,
      std::conditional_t<IsDirectJump, uint32_t, MemoryOperand<uint32_t>>>;

 public:
  JmpInstruction(Operand dst);
  void execute(Cpu& cpu) override;
  std::string str() override;

 private:
  Operand dst_;
};

// template <bool IsRelativeJump>
// class JmpNearInstruction : public Instruction {
//  public:
//   JmpNearInstruction(uint16_t disp);
//   void execute(Cpu& cpu) override;
//   std::string str() override;

//  private:
//   uint16_t disp_;
// };

// class JmpFarAbsoluteDirectInstruction : public Instruction {
//  public:
//   JmpFarAbsoluteDirectInstruction(uint16_t selector, uint16_t offset);
//   void execute(Cpu& cpu) override;
//   std::string str() override;

//  private:
//   uint16_t selector_;
//   uint16_t offset_;
// };

}  // namespace emu::cpu::insts
