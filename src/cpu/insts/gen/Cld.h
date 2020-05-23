#pragma once

#include <fmt/format.h>

#include "cpu/insts/Instruction.h"

namespace emu::cpu::insts {

class Cld : public Instruction {
 public:
  Cld() {}

  void execute(Cpu& cpu) override;

  std::string str() const noexcept override {
    return fmt::format("CLD");
  }

 private:
};

}  // namespace emu::cpu::insts
