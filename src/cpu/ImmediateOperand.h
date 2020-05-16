#pragma once

#include <string>

namespace emu::cpu {

template <typename T>
class ImmediateOperand {
 public:
  using type = T;

  ImmediateOperand(T imm);

  T r() noexcept;

  std::string str() const noexcept;

 private:
  T imm_;
};

}  // namespace emu::cpu
