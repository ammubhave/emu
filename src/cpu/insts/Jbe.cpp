#include "gen/Jbe.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

template <typename Op1>
void Jbe<Op1>::execute(Cpu& cpu) {
  if (CF || ZF) {
    IP = op1_.r();
  }
}

}  // namespace emu::cpu::insts

#include "gen/Jbe.cpp"
