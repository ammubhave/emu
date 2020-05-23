#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

#include "cpu/Types.h"

namespace emu::cpu {

class Cpu;

template <typename T>
class MemoryOperand {
 public:
  using type = T;

  MemoryOperand(Cpu& cpu, Segment seg, uint16_t offset);

  T r();

  void w(T v);

  std::string str() const noexcept;

  uint16_t effectiveAddress();

 private:
  Cpu& cpu_;
  Segment seg_;
  uint16_t offset_;
};

}  // namespace emu::cpu
