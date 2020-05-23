#include "gen/Jnb.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

template <typename Op1>
void Jnb<Op1>::execute(Cpu& cpu) {
  Jcc(cpu, op1_, [&]() { return !CF; });
}

}  // namespace emu::cpu::insts

#include "gen/Jnb.cpp"
