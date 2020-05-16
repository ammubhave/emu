#pragma once

#include "cpu/insts/Instruction.h"

namespace emu::cpu::insts {

class CliInstruction : public Instruction {
 public:
  CliInstruction();
  void execute(Cpu& cpu) override;
  std::string str() override;
};

}  // namespace emu::cpu::insts
