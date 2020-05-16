#pragma once

#include <cstdint>

#include "cpu/insts/Instruction.h"

namespace emu::cpu::insts {

class JmpNearRelativeInstruction : public Instruction {
 public:
  JmpNearRelativeInstruction(uint16_t disp);
  void execute(Cpu& cpu) override;
  std::string str() override;

 private:
  uint16_t disp_;
};

class JmpFarAbsoluteDirectInstruction : public Instruction {
 public:
  JmpFarAbsoluteDirectInstruction(uint16_t selector, uint16_t offset);
  void execute(Cpu& cpu) override;
  std::string str() override;

 private:
  uint16_t selector_;
  uint16_t offset_;
};

}  // namespace emu::cpu::insts
