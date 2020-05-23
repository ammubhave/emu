#pragma once

#include <fmt/format.h>

#include "cpu/insts/Instruction.h"

namespace emu::cpu::insts {

class Hlt : public Instruction {
 public:
  Hlt() {}

  void execute(Cpu& cpu) override;

  std::string str() const noexcept override {
    return fmt::format("HLT");
  }

 private:
};

}  // namespace emu::cpu::insts
