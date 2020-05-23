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

  SegmentRegisterOperand(Cpu& cpu, uint8_t idx);

  uint16_t r() const noexcept;

  void w(uint16_t v) noexcept;

  std::string str() const noexcept;

  Segment seg() const noexcept;

 private:
  Cpu& cpu_;
  uint8_t idx_;
};

}  // namespace emu::cpu
