#pragma once

#include <string>

namespace emu::cpu {

class Cpu;

namespace insts {

class Instruction {
 public:
  virtual ~Instruction() = default;
  virtual void execute(Cpu& cpu) = 0;
  virtual std::string str() const noexcept = 0;
};

}  // namespace insts
}  // namespace emu::cpu
