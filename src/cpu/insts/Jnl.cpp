#include "gen/Jnl.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

template <typename Op1>
void Jnl<Op1>::execute(Cpu& cpu) {
  if (SF == OF) {
    IP = op1_.r();
  }
}

}  // namespace emu::cpu::insts

#include "gen/Jnl.cpp"
