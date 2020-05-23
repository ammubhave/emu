#include "gen/Sti.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

void Sti::execute(Cpu& cpu) { IF = true; }

}  // namespace emu::cpu::insts
