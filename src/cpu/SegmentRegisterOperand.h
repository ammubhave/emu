#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

#include "cpu/Types.h"

namespace emu::cpu {

class Cpu;

class SegmentRegisterOperand {
 public:
  using type = uint16_t;

  SegmentRegisterOperand(Cpu& cpu, size_t idx);

  uint16_t r() noexcept;

  void w(uint16_t v) noexcept;

  std::string str() const noexcept;

  Segment seg() noexcept;

 private:
  Cpu& cpu_;
  size_t idx_;
};

}  // namespace emu::cpu
