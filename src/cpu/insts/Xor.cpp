#include "gen/Xor.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

template <typename Op1, typename Op2>
void Xor<Op1, Op2>::execute(Cpu& cpu) {
  static_assert(std::is_same_v<typename Op1::type, typename Op2::type>);
  arith<true>(
      cpu, op1_, op2_,
      [&](typename Op1::type a, typename Op2::type b,
          bool) -> std::tuple<typename Op1::type, bool, bool, bool> {
        return std::make_tuple(a ^ b, false, false, false);
      });
}

}  // namespace emu::cpu::insts

#include "gen/Xor.cpp"
