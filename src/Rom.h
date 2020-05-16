#pragma once

#include <array>
#include <string_view>

#include "Bus.h"

template <size_t T>
class Rom : public MemoryBusOperations {
 public:
  Rom(std::string_view filename);

 private:
  uint8_t readMemory(size_t address) final;
  void writeMemory(size_t address, uint8_t value) final;

  const std::array<uint8_t, T> rom_;
};

#include "Rom-inl.h"
