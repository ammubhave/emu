#include "gen/Mov.h"

#include <fmt/format.h>

#include "cpu/Cpu.h"
#include "cpu/operands/ImmediateOperand.h"
#include "cpu/operands/RegisterOperand.h"
#include "cpu/operands/SegmentRegisterOperand.h"

namespace emu::cpu::insts {

template <typename Op1, typename Op2>
void Mov<Op1, Op2>::execute(Cpu& cpu) {
  if constexpr (std::is_same_v<Op1, SegmentRegisterOperand>) {
    assert(op1_.seg().index() != Segment::CS.index());
    // TODO: If op1 is SS, inhibit interrupts for one instruction.
  }

  op1_.w(op2_.r());

  if constexpr (std::is_same_v<Op1, SegmentRegisterOperand>) {
    // TODO: Refresh descriptor
  }
}

}  // namespace emu::cpu::insts

#include "gen/Mov.cpp"
