#pragma once

namespace emu::cpu {

class Cpu;

class InstructionFetcher {
 public:
  InstructionFetcher(Cpu& cpu);

  template <typename T>
  T peek();
  template <typename T>
  T next();

 private:
  Cpu& cpu_;
};

}  // namespace emu::cpu
