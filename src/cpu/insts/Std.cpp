#include "gen/Std.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

void Std::execute(Cpu& cpu) { DF = true; }

}  // namespace emu::cpu::insts
