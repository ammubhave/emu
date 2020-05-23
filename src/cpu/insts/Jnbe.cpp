#include "gen/Jnbe.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

template <typename Op1>
void Jnbe<Op1>::execute(Cpu& cpu) {
  Jcc(cpu, op1_, [&]() { return !CF && !ZF; });
}

}  // namespace emu::cpu::insts

#include "gen/Jnbe.cpp"
