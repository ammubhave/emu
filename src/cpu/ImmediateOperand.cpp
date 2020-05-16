#include "ImmediateOperand.h"

#include <fmt/format.h>

#include <cstdint>

namespace emu::cpu {

template <typename T>
ImmediateOperand<T>::ImmediateOperand(T imm) : imm_{imm} {}

template <typename T>
T ImmediateOperand<T>::r() noexcept {
  return imm_;
}

template <typename T>
std::string ImmediateOperand<T>::str() const noexcept {
  return fmt::format("0x{:x}", imm_);
}

template class ImmediateOperand<uint8_t>;
template class ImmediateOperand<uint16_t>;

}  // namespace emu::cpu
