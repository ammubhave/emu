#include "gen/Sahf.h"

#include <iostream>

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

template <typename Op1>
void Sahf<Op1>::execute(Cpu& cpu) {
  cpu.regs_.flags.value = (cpu.regs_.flags.value & 0xff00) | op1_.r();
}

}  // namespace emu::cpu::insts

#include "gen/Sahf.cpp"
