#include "gen/Stc.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

void Stc::execute(Cpu& cpu) { CF = true; }

}  // namespace emu::cpu::insts
