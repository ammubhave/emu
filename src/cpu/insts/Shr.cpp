#include "gen/Shr.h"

#include <iostream>

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

template <typename Op1, typename Op2>
void Shr<Op1, Op2>::execute(Cpu& cpu) {
  rotate_shift<true>(
      cpu, op1_, op2_,
      [&](typename Op1::type a, uint16_t count,
          bool) -> std::tuple<typename Op1::type, bool, bool> {
        if (count > sizeof(a) * 8) {
          return std::make_tuple(0, false, false);
        }
        if (count == sizeof(a) * 8) {
          return std::make_tuple(0, msb(a), false);
        }
        decltype(a) result = a >> count;
        bool cf = lsb(a >> (count - 1));
        return std::make_tuple(result, cf, msb(a));
      });
}

}  // namespace emu::cpu::insts

#include "gen/Shr.cpp"
