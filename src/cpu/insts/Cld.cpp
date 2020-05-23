#include "gen/Cld.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

void Cld::execute(Cpu& cpu) { DF = false; }

}  // namespace emu::cpu::insts
