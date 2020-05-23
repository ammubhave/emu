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
  void enableDebug() { debugEnabled_ = true; }

  template <typename T>
  T readPhysicalMemory(size_t addr);

  template <typename T>
  void writePhysicalMemory(size_t addr, T v);

  template <typename T>
  T readLogicalMemory(Segment seg, uint16_t offset);

  template <typename T>
  void writeLogicalMemory(Segment seg, uint16_t offset, T v);

  size_t instructionFetchAddress() const;

  size_t paddr(Segment segment, uint16_t offset,
               MemoryOperationType memoryOperationType) const;

  Registers regs_{};
  Bus& bus_;
  bool debugEnabled_{false};
};

}  // namespace emu::cpu

#include "Cpu-inl.h"
