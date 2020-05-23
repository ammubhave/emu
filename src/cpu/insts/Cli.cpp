#include "gen/Cli.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

void Cli::execute(Cpu& cpu) { IF = false; }

}  // namespace emu::cpu::insts
