#pragma once

#include <array>

#include "Bus.h"

namespace emu::peripherals::cmos {

class Cmos final : public IOBusOperations {
 public:
  Cmos();

  uint8_t readPort(uint16_t port);

  void writePort(uint16_t port, uint8_t value);

  bool nmiDisabled{false};

 private:
  void recomputeChecksum();

  union {
    std::array<uint8_t, 128> ram_;
  };

  uint8_t selectedRegister_{0xd};
};

}  // namespace emu::peripherals::cmos
