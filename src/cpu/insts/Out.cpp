#include "gen/Out.h"

#include <fmt/format.h>

#include "cpu/Cpu.h"
#include "cpu/operands/ImmediateOperand.h"
#include "cpu/operands/RegisterOperand.h"
#include "cpu/operands/SegmentRegisterOperand.h"

namespace emu::cpu::insts {

template <typename Op1, typename Op2>
void Out<Op1, Op2>::execute(Cpu& cpu) {
  const auto dest = static_cast<uint16_t>(op1_.r());
  const auto value = op2_.r();

  cpu.bus_.writePort(dest, static_cast<uint8_t>(value));
  if constexpr (sizeof(typename Op2::type) == 2) {
    cpu.bus_.writePort(dest + 1, static_cast<uint8_t>(value >> 8));
  }
}

}  // namespace emu::cpu::insts

#include "gen/Out.cpp"
