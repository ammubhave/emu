#pragma once

#include "Bus.h"
#include "Registers.h"
#include "Types.h"
#include "cpu/InstructionSet.h"

namespace emu::cpu {

template <typename T>
class ImmediateOperand;

class Cpu {
 public:
  Cpu(Bus& bus) noexcept;

  void step() noexcept;

  template <typename T>
  T readPhysicalMemory(size_t addr) {
    T v;
    std::array<uint8_t, sizeof(T)> data;
    for (size_t i = 0; i < sizeof(T); ++i) {
      data[i] = readPhysicalMemory<uint8_t>(addr + i);
    }
    std::memcpy(&v, data.data(), data.size());
    return v;
  }

  template <typename T>
  void writePhysicalMemory(size_t addr, T v) {
    for (size_t i = 0; i < sizeof(T); ++i) {
      writePhysicalMemory(addr++, static_cast<uint8_t>(v));
      v >>= 8;
    }
  }

  template <typename T>
  T readLogicalMemory(Segment seg, uint16_t offset, size_t delta = 0);

  template <typename T>
  void writeLogicalMemory(Segment seg, uint16_t offset, T v, size_t delta = 0);

  size_t instructionFetchAddress() const;

  size_t paddr(Segment segment, uint16_t offset,
               MemoryOperationType memoryOperationType) const;

  Registers regs_{};
  Bus& bus_;
};

template <>
uint8_t Cpu::readPhysicalMemory(size_t addr) {
  return bus_.readMemory(addr);
}

template <>
void Cpu::writePhysicalMemory(size_t addr, uint8_t v) {
  bus_.writeMemory(addr, static_cast<uint8_t>(v));
}

}  // namespace emu::cpu
