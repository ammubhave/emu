#include "gen/Jns.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

template <typename Op1>
void Jns<Op1>::execute(Cpu& cpu) {
  if (!SF) {
    IP = op1_.r();
  }
}

}  // namespace emu::cpu::insts

#include "gen/Jns.cpp"
