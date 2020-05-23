#include "MemoryOperand.h"

#include <fmt/format.h>

#include "cpu/Cpu.h"
#include "cpu/Types.h"

namespace emu::cpu {

template <typename T>
MemoryOperand<T>::MemoryOperand(Cpu& cpu, Segment seg, uint16_t offset)
    : cpu_{cpu}, seg_{seg}, offset_{offset} {}

template <typename T>
T MemoryOperand<T>::r() {
  return cpu_.readLogicalMemory<T>(seg_, offset_);
}

template <typename T>
void MemoryOperand<T>::w(T v) {
  cpu_.writeLogicalMemory<T>(seg_, offset_, v);
}

template <typename T>
uint16_t MemoryOperand<T>::effectiveAddress() {
  return offset_;
}

template <typename T>
std::string MemoryOperand<T>::str() const noexcept {
  return fmt::format("[{}:0x{:x}]", seg_.str(), offset_);
}

template class MemoryOperand<uint8_t>;
template class MemoryOperand<uint16_t>;

}  // namespace emu::cpu
