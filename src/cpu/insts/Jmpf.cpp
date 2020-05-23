#include "gen/Jmpf.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"
#include "cpu/operands/ImmediateOperand.h"

namespace emu::cpu::insts {

template <typename Op>
void Jmpf<Op>::execute(Cpu& cpu) {
  const auto v = op1_.r();

  struct {
    uint16_t offset;
    uint16_t segmentSelector;
  } destination{static_cast<uint16_t>(v), static_cast<uint16_t>(v >> 16)};

  uint16_t tempIp = destination.offset;
  if (!IsWithinCodeSegmentLimits(tempIp)) {
    Exception(EX_GP(0));
  }
  CS = destination.segmentSelector;
  IP = tempIp;
}

}  // namespace emu::cpu::insts

#include "gen/Jmpf.cpp"
