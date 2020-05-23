#include "gen/Test.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

template <typename Op1, typename Op2>
void Test<Op1, Op2>::execute(Cpu& cpu) {
  static_assert(std::is_same_v<typename Op1::type, typename Op2::type>);
  typename Op1::type result = op1_.r() & op2_.r();
  OF = false;
  CF = false;
  SF = calc_sf(result);
  ZF = calc_zf(result);
  PF = calc_pf(result);
}

}  // namespace emu::cpu::insts

#include "gen/Test.cpp"
