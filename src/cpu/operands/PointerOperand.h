#pragma once

#include <string>

#include "cpu/operands/MemoryOperand.h"

namespace emu::cpu {

class Cpu;

template <typename T>
class PointerOperand {
 public:
  using type = T;

  PointerOperand(Cpu& cpu, uint32_t ptr);

  T r() noexcept;

  std::string str() const noexcept;

 private:
  MemoryOperand<T> memOp_;
};

}  // namespace emu::cpu
