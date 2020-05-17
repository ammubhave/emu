#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

#include "cpu/Types.h"

namespace emu::cpu {

class Cpu;

template <typename T = uint8_t>
class MemoryOperand {
 public:
  using type = T;

  MemoryOperand(Cpu& cpu, Segment seg, uint16_t offset);

  T r();

  template <typename S>
  S r(size_t delta) {
    S v;
    std::array<uint8_t, sizeof(S)> data;
    for (size_t i = 0; i < data.size(); i++) {
      data[i] = r8(i);
    }
    std::memcpy(&v, data.data(), data.size());
    return v;
  }

  void w(T v);

  template <typename S>
  void w(size_t delta, const S& v) {
    std::array<uint8_t, sizeof(S)> data;
    std::memcpy(data.data(), &v, data.size());
    for (size_t i = 0; i < data.size(); i++) {
      w8(data[i], i);
    }
  }

  std::string str() const;

  uint16_t effectiveAddress();

 private:
  uint8_t r8(size_t delta = 0);
  void w8(uint8_t v, size_t delta = 0);

  Cpu& cpu_;
  Segment seg_;
  uint16_t offset_;
};

}  // namespace emu::cpu
