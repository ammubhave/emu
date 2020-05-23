#include "gen/Clc.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

void Clc::execute(Cpu& cpu) { CF = false; }

}  // namespace emu::cpu::insts
