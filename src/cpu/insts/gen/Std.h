#pragma once

#include <fmt/format.h>

#include "cpu/insts/Instruction.h"

namespace emu::cpu::insts {

class Std : public Instruction {
 public:
  Std() {}

  void execute(Cpu& cpu) override;

  std::string str() const noexcept override {
    return fmt::format("STD");
  }

 private:
};

}  // namespace emu::cpu::insts
