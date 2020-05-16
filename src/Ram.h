#pragma once

#include <array>

#include "Bus.h"

template <size_t T>
class Ram : public MemoryBusOperations {
  private:
  uint8_t readMemory(size_t address) final;
  void writeMemory(size_t address, uint8_t value) final;

  std::array<uint8_t, T> ram_;
};

#include "Ram-inl.h"
