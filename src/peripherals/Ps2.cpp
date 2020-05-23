#include "Ps2.h"

#include <iostream>

namespace emu::peripherals::ps2 {

Ps2::Ps2() {}

uint8_t Ps2::readPort(uint16_t port) {
  switch (port) {
    case 0:
      return 0;
    case 1:
      return 0;
    case 2:
      return 0;
    case 3:
      return 0;
  }
  __builtin_unreachable();
}

void Ps2::writePort(uint16_t port, uint8_t value) {
  switch (port) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    default:
      __builtin_unreachable();
  }
}

}  // namespace emu::peripherals::ps2
