#pragma once

#include <string>

namespace emu::cpu {

class Cpu;

template <typename T>
class RelativeOffsetOperand {
 public:
  using type = uint16_t;

  RelativeOffsetOperand(Cpu& cpu, T imm);

  uint16_t r() const noexcept;

  std::string str() const noexcept;

 private:
  Cpu& cpu_;
  T imm_;
};

}  // namespace emu::cpu
