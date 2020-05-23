#pragma once

#include <fmt/format.h>

#include "cpu/insts/Instruction.h"

namespace emu::cpu::insts {

class Clc : public Instruction {
 public:
  Clc() {}

  void execute(Cpu& cpu) override;

  std::string str() const noexcept override {
    return fmt::format("CLC");
  }

 private:
};

}  // namespace emu::cpu::insts
