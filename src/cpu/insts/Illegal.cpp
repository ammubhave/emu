#include "Illegal.h"

#include <fmt/format.h>

#include "cpu/Cpu.h"

namespace emu::cpu::insts {

Illegal::Illegal(FetchedInstruction inst) : inst_{inst} {}

void Illegal::execute(Cpu& cpu) { throw new std::exception(); }

std::string Illegal::str() const noexcept {
  return fmt::format("Illegal Instruction: Opcode: 0x{:x}", inst_.opcode.po);
}

}  // namespace emu::cpu::insts
