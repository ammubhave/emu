#include "PointerOperand.h"

#include <fmt/format.h>

#include <cstdint>

#include "cpu/Cpu.h"

namespace emu::cpu {

template <typename T>
PointerOperand<T>::PointerOperand(uint32_t ptr)
    : memOp_{cpu, Segment{static_cast<uint16_t>(ptr >>)}} {}

template <typename T>
T PointerOperand<T>::r() noexcept {
  return
}

template <typename T>
std::string PointerOperand<T>::str() const noexcept {
  return fmt::format("0x{:x}", imm_);
}

template class PointerOperand<uint8_t>;
template class PointerOperand<uint16_t>;

}  // namespace emu::cpu
