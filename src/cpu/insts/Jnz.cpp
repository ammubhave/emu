#include "gen/Jnz.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

template <typename Op1>
void Jnz<Op1>::execute(Cpu& cpu) {
  if (!ZF) {
    IP = op1_.r();
  }
}

}  // namespace emu::cpu::insts

#include "gen/Jnz.cpp"
