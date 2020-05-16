#include "ModRm.h"

#include "cpu/InstructionFetcher.h"
#include "cpu/InstructionSet.h"
#include "cpu/Opcode.h"

namespace emu::cpu {

ModRm::ModRm(InstructionFetcher fetcher, Opcode opcode) {
  if (!hasModRm(opcode)) {
    return;
  }

  const auto value = fetcher.next<uint8_t>();
  rm = value & 7;
  reg = o = (value >> 3) & 7;
  mod = value >> 6;

  if ((mod == 0 && rm == 6) || mod == 2) {
    disp = fetcher.next<uint16_t>();
  } else if (mod == 1) {
    disp = fetcher.next<uint8_t>();
  }

  switch (opcode.pf[1]) {
    case 0x2e:
      segmentOverride = Segment::CS;
      break;
    case 0x36:
      segmentOverride = Segment::SS;
      break;
    case 0x3e:
      segmentOverride = Segment::DS;
      break;
    case 0x26:
      segmentOverride = Segment::ES;
      break;
  }
}

}  // namespace emu::cpu
