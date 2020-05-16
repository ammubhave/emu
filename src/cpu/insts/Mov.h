#pragma once

#include "cpu/insts/Instruction.h"

namespace emu::cpu::insts {

template <typename Op1, typename Op2>
class MovInstruction : public Instruction {
  static_assert(std::is_same_v<typename Op1::type, typename Op2::type>);

 public:
  MovInstruction(Op1 dst, Op2 src);
  void execute(Cpu& cpu) override;
  std::string str() override;

 private:
  Op1 dst_;
  Op2 src_;
};

}  // namespace emu::cpu::insts

#include "Mov-inl.h"
