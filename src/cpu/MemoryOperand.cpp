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
uint8_t MemoryOperand<T>::r8(size_t delta) {
  return cpu_.readLogicalMemory<uint8_t>(seg_, offset_, delta);
}

template <typename T>
void MemoryOperand<T>::w8(uint8_t v, size_t delta) {
  cpu_.writeLogicalMemory<T>(seg_, offset_, v, delta);
}

template <typename T>
std::string MemoryOperand<T>::str() const {
  return fmt::format("[{}:0x{:x}]", ([&]() {
                       switch (seg_) {
                         case Segment::ES:
                           return "ES";
                         case Segment::CS:
                           return "CS";
                         case Segment::SS:
                           return "SS";
                         case Segment::DS:
                           return "DS";
                       }
                       __builtin_unreachable();
                     })(),
                     offset_);
}

template class MemoryOperand<uint8_t>;
template class MemoryOperand<uint16_t>;

}  // namespace emu::cpu
