#pragma once

#include <cstdint>
#include <optional>

#include "cpu/Types.h"

namespace emu::cpu {

class InstructionFetcher;
class Opcode;

class ModRm {
 public:
  ModRm(InstructionFetcher fetcher, Opcode opcode);

  uint8_t rm;
  uint8_t reg;
  uint8_t o;
  uint8_t mod;
  uint16_t disp;
  std::optional<Segment> segmentOverride;
};

}  // namespace emu::cpu
