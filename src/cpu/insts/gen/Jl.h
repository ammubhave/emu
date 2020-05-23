#pragma once

#include <fmt/format.h>

#include "cpu/insts/Instruction.h"

namespace emu::cpu::insts {

template <typename Op1>
class Jl : public Instruction {
 public:
  Jl(Op1 op1) : op1_{op1} {}

  void execute(Cpu& cpu) override;

  std::string str() const noexcept override {
    return fmt::format("JL	{}", op1_.str());
  }

 private:
  Op1 op1_;
};

}  // namespace emu::cpu::insts
