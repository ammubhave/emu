#include "RelativeOffsetOperand.h"

#include <fmt/format.h>

#include <bit>
#include <cstdint>

#include "cpu/Cpu.h"

namespace emu::cpu {

template <typename T>
RelativeOffsetOperand<T>::RelativeOffsetOperand(Cpu& cpu, T imm)
    : cpu_{cpu}, imm_{imm} {}

template <typename T>
uint16_t RelativeOffsetOperand<T>::r() const noexcept {
  uint16_t addr = cpu_.regs_.ip;
  if constexpr (sizeof(T) == 1) {
    addr += fmt::internal::bit_cast<uint16_t>(
        static_cast<int16_t>(fmt::internal::bit_cast<int8_t>(imm_)));
  } else {
    addr += imm_;
  }
  return addr;
}

template <typename T>
std::string RelativeOffsetOperand<T>::str() const noexcept {
  return fmt::format("IP + 0x{:x}", imm_);
}

template class RelativeOffsetOperand<uint8_t>;
template class RelativeOffsetOperand<uint16_t>;

}  // namespace emu::cpu
