#pragma once

#include <fmt/format.h>

#include "cpu/insts/Instruction.h"

namespace emu::cpu::insts {

template <typename Op1, typename Op2>
class Rcl : public Instruction {
 public:
  Rcl(Op1 op1, Op2 op2) : op1_{op1}, op2_{op2} {}

  void execute(Cpu& cpu) override;

  std::string str() const noexcept override {
    return fmt::format("RCL	{}, {}", op1_.str(), op2_.str());
  }

 private:
  Op1 op1_;

  Op2 op2_;
};

}  // namespace emu::cpu::insts
