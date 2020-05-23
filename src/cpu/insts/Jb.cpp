#include "gen/Jb.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

template <typename Op1>
void Jb<Op1>::execute(Cpu& cpu) {
  if (CF) {
    IP = op1_.r();
  }
}

}  // namespace emu::cpu::insts

#include "gen/Jb.cpp"
