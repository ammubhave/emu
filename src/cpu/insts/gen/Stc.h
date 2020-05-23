#pragma once

#include <fmt/format.h>

#include "cpu/insts/Instruction.h"

namespace emu::cpu::insts {

class Stc : public Instruction {
 public:
  Stc() {}

  void execute(Cpu& cpu) override;

  std::string str() const noexcept override {
    return fmt::format("STC");
  }

 private:
};

}  // namespace emu::cpu::insts
