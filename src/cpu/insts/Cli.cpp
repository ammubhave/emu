#include "Cli.h"

#include "cpu/Cpu.h"

namespace emu::cpu::insts {

CliInstruction::CliInstruction() {}

void CliInstruction::execute(Cpu& cpu) {
  // TODO: Check if protected mode ring zero
  cpu.regs_.flags._if = false;
}

std::string CliInstruction::str() { return "CLI"; }

}  // namespace emu::cpu::insts
