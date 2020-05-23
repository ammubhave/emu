#include "Registers.h"

#include <iostream>
template <>
uint8_t& Registers::reg(size_t i) {
  assert((i & ~7) == 0);
  i = ((i & 3) << 1) + (i >> 2);
  return generalPurposeRegisters8[i];
}

template <>
uint16_t& Registers::reg(size_t i) {
  return generalPurposeRegisters[i];
}

uint16_t& Registers::sreg(size_t i) {
  assert(i < 4);
  return segmentRegisters[i].selector.value;
}
