#include "Opcode.h"

#include "cpu/InstructionFetcher.h"
#include "cpu/RegisterOperand.h"

namespace emu::cpu {

Opcode::Opcode(InstructionFetcher fetcher) {
  uint8_t value;

prefixLoop:
  value = fetcher.next<uint8_t>();
  switch (value) {
    case 0xf0:
    case 0xf2:
    case 0xf3:
      pf[0] = value;
      goto prefixLoop;
    case 0x26:
    case 0x2e:
    case 0x36:
    case 0x3e:
    case 0x64:
    case 0x65:
      pf[1] = value;
      goto prefixLoop;
    case 0x66:
      pf[2] = value;
      goto prefixLoop;
    case 0x67:
      pf[3] = value;
      goto prefixLoop;
  }

  if (value == 0x0f) {
    of = true;
    value = fetcher.next<uint8_t>();
  }

  po = value;
  r = sre30 = po & 7;
  w = po & 1;
  d = s = po & 2;
  tttn = po & 0xf;
  sr = (po >> 3) & 3;
  sre33 = (po >> 3) & 7;
  mf = (po >> 1) & 3;

  if (of && (po == 0x38 || po == 0x3a)) {
    so = fetcher.next<uint8_t>();
  }
}

template <typename T>
RegisterOperand<T> Opcode::op_r(Cpu& cpu) const {
  return RegisterOperand<T>(cpu, r);
}

template RegisterOperand<uint8_t> Opcode::op_r(Cpu&) const;
template RegisterOperand<uint16_t> Opcode::op_r(Cpu&) const;

}  // namespace emu::cpu
