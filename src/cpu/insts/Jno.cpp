#include "gen/Jno.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

template <typename Op1>
void Jno<Op1>::execute(Cpu& cpu) {
  if (!OF) {
    IP = op1_.r();
  }
}

}  // namespace emu::cpu::insts

#include "gen/Jno.cpp"
