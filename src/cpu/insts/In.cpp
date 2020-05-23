#include "gen/In.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

template <typename Op1, typename Op2>
void In<Op1, Op2>::execute(Cpu& cpu) {
  const auto src = static_cast<uint16_t>(op2_.r());

  typename Op1::type value;
  value = cpu.bus_.readPort(src);
  if constexpr (sizeof(typename Op1::type) == 2) {
    value |= static_cast<decltype(value)>(cpu.bus_.readPort(src + 1)) << 8;
  }
  op1_.w(value);
}

}  // namespace emu::cpu::insts

#include "gen/In.cpp"
