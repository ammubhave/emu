#include <fmt/format.h>

#include "Mov.h"
#include "cpu/Cpu.h"
#include "cpu/SegmentRegisterOperand.h"

namespace emu::cpu::insts {

template <typename Op1, typename Op2>
MovInstruction<Op1, Op2>::MovInstruction(Op1 dst, Op2 src)
    : dst_{dst}, src_{src} {}

template <typename Op1, typename Op2>
void MovInstruction<Op1, Op2>::execute(Cpu& cpu) {
  auto Source = src_;
  auto Destination = dst_;

  if constexpr (std::is_same_v<Op1, SegmentRegisterOperand>) {
    assert(dst_.seg() != Segment::CS);
    // TODO: If op1 is SS, inhibit interrupts for one instruction.
  }

  dst_.w(src_.r());

  if constexpr (std::is_same_v<Op1, SegmentRegisterOperand>) {
    // TODO: Refresh descriptor
  }
}

template <typename Op1, typename Op2>
std::string MovInstruction<Op1, Op2>::str() {
  return fmt::format("MOV\t{}, {}", dst_.str(), src_.str());
}

}  // namespace emu::cpu::insts
