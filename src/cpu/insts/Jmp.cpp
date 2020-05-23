#include "gen/Jmp.h"

#include <fmt/format.h>

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"
#include "cpu/operands/ImmediateOperand.h"
#include "cpu/operands/RelativeOffsetOperand.h"

namespace emu::cpu::insts {

template <typename Op1>
void Jmp<Op1>::execute(Cpu& cpu) {
  const auto tempIp = op1_.r();

  if (!IsWithinCodeSegmentLimits(tempIp)) {
    Exception(EX_GP(0));
  }

  IP = tempIp;
}

}  // namespace emu::cpu::insts

#include "gen/Jmp.cpp"
