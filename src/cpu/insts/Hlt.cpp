#include "gen/Hlt.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

void Hlt::execute(Cpu& cpu) { throw new std::runtime_error("Halted"); }

}  // namespace emu::cpu::insts
