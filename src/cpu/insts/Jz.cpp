#include "gen/Jz.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

template <typename Op1>
void Jz<Op1>::execute(Cpu& cpu) {
  if (ZF) {
    IP = op1_.r();
  }
}

}  // namespace emu::cpu::insts

#include "gen/Jz.cpp"
