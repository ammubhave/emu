#pragma once

#include <fmt/format.h>

#include "cpu/insts/Instruction.h"

namespace emu::cpu::insts {

class Cli : public Instruction {
 public:
  Cli() {}

  void execute(Cpu& cpu) override;

  std::string str() const noexcept override {
    return fmt::format("CLI");
  }

 private:
};

}  // namespace emu::cpu::insts
