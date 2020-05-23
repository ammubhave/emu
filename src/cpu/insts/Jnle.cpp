#include "gen/Jnle.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

template <typename Op1>
void Jnle<Op1>::execute(Cpu& cpu) {
  if (!ZF && SF == OF) {
    IP = op1_.r();
  }
}

}  // namespace emu::cpu::insts

#include "gen/Jnle.cpp"
