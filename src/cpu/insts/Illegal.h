#pragma once

#include <cstdint>

#include "cpu/InstructionFetcher.h"
#include "cpu/insts/Instruction.h"

namespace emu::cpu::insts {

class Illegal : public Instruction {
 public:
  Illegal(FetchedInstruction inst);
  void execute(Cpu& cpu) override;
  std::string str() const noexcept override;

 private:
  FetchedInstruction inst_;
};

}  // namespace emu::cpu::insts
