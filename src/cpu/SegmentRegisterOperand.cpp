#include "SegmentRegisterOperand.h"

#include "cpu/Cpu.h"

namespace emu::cpu {

SegmentRegisterOperand::SegmentRegisterOperand(Cpu& cpu, size_t idx)
    : cpu_{cpu}, idx_{idx} {}

uint16_t SegmentRegisterOperand::r() noexcept { return cpu_.regs_.sreg(idx_); }

void SegmentRegisterOperand::w(uint16_t v) noexcept {
  cpu_.regs_.sreg(idx_) = v;
}

std::string SegmentRegisterOperand::str() const noexcept {
  switch (idx_) {
    case 0:
      return "ES";
    case 1:
      return "CS";
    case 2:
      return "SS";
    case 3:
      return "DS";
  }
  __builtin_unreachable();
}

Segment SegmentRegisterOperand::seg() noexcept { return (Segment)idx_; };

}  // namespace emu::cpu
