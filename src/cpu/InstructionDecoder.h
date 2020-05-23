#pragma once

#include <memory>

namespace emu::cpu {

class Cpu;
class Immediate;
class ModRm;
class Opcode;
class FetchedInstruction;

namespace insts {
class Instruction;
}

class InstructionDecoder {
 public:
  InstructionDecoder(Cpu& cpu);

  std::unique_ptr<insts::Instruction> decode(
      FetchedInstruction fetchedInstruction);

 private:
  Cpu& cpu_;
};

}  // namespace emu::cpu
