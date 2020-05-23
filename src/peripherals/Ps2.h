#pragma once

#include <array>

#include "Bus.h"

namespace emu::peripherals::ps2 {

class Ps2 final : public IOBusOperations {
 public:
  Ps2();

  uint8_t readPort(uint16_t port);

  void writePort(uint16_t port, uint8_t value);

 private:
};

}  // namespace emu::peripherals::ps2
