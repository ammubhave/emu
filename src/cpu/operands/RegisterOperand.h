#pragma once

#include <cstddef>
#include <string>

namespace emu::cpu {

class Cpu;

template <typename T>
class RegisterOperand {
 public:
  using type = T;

  RegisterOperand(Cpu& cpu, size_t idx);

  T r() const noexcept;

  void w(T v) noexcept;

  std::string str() const noexcept;

 private:
  Cpu& cpu_;
  size_t idx_;
};

}  // namespace emu::cpu
