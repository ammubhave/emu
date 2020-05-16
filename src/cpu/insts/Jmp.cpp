#include "Jmp.h"

#include <fmt/format.h>

#include "cpu/Cpu.h"

namespace emu::cpu::insts {

JmpNearRelativeInstruction::JmpNearRelativeInstruction(uint16_t disp)
    : disp_{disp} {}

void JmpNearRelativeInstruction::execute(Cpu& cpu) { cpu.regs_.ip += disp_; }

std::string JmpNearRelativeInstruction::str() {
  return fmt::format("JMP\tIP + 0x{:x}", disp_);
}

JmpFarAbsoluteDirectInstruction::JmpFarAbsoluteDirectInstruction(
    uint16_t selector, uint16_t offset)
    : selector_{selector}, offset_{offset} {}

void JmpFarAbsoluteDirectInstruction::execute(Cpu& cpu) {
  cpu.regs_.cs = selector_;
  cpu.regs_.ip = offset_;
  // TODO: cpu.refresh_descriptor(SegmentRegister::CS);
}

std::string JmpFarAbsoluteDirectInstruction::str() {
  return fmt::format("JMP\t0x{:x}:0x{:x}", selector_, offset_);
}

}  // namespace emu::cpu::insts
