#include "gen/Lahf.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

template <typename Op1>
void Lahf<Op1>::execute(Cpu& cpu) {
  op1_.w(static_cast<uint8_t>(cpu.regs_.flags.value));
}

}  // namespace emu::cpu::insts

#include "gen/Lahf.cpp"
