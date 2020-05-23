#include "gen/Jo.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

template <typename Op1>
void Jo<Op1>::execute(Cpu& cpu) {
  if (OF) {
    IP = op1_.r();
  }
}

}  // namespace emu::cpu::insts

#include "gen/Jo.cpp"