#include "gen/Jnp.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

template <typename Op1>
void Jnp<Op1>::execute(Cpu& cpu) {
  if (!PF) {
    IP = op1_.r();
  }
}

}  // namespace emu::cpu::insts

#include "gen/Jnp.cpp"
