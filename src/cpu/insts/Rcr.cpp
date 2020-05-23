#include "gen/Rcr.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

template <typename Op1, typename Op2>
void Rcr<Op1, Op2>::execute(Cpu& cpu) {
  throw new std::exception();
}

}  // namespace emu::cpu::insts

#include "gen/Rcr.cpp"