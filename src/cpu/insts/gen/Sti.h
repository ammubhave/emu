#pragma once

#include <fmt/format.h>

#include "cpu/insts/Instruction.h"

namespace emu::cpu::insts {

class Sti : public Instruction {
 public:
  Sti() {}

  void execute(Cpu& cpu) override;

  std::string str() const noexcept override {
    return fmt::format("STI");
  }

 private:
};

}  // namespace emu::cpu::insts
