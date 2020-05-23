#include "InstructionFetcher.h"

#include <cstdint>

#include "cpu/Cpu.h"

namespace emu::cpu {

InstructionFetcher::InstructionFetcher(Cpu& cpu) : cpu_{cpu} {}

template <typename T>
T InstructionFetcher::peek() {
  return cpu_.readLogicalMemory<T>(Segment::CS, cpu_.regs_.ip);
}
template uint8_t InstructionFetcher::peek();
template uint16_t InstructionFetcher::peek();
template uint32_t InstructionFetcher::peek();

template <typename T>
T InstructionFetcher::next() {
  auto value = peek<T>();
  cpu_.regs_.ip += sizeof(T);
  return value;
}

template uint8_t InstructionFetcher::next();
template uint16_t InstructionFetcher::next();
template uint32_t InstructionFetcher::next();

FetchedInstruction InstructionFetcher::fetchNextInstruction() {
  return FetchedInstruction{*this};
}

FetchedInstruction::FetchedInstruction(InstructionFetcher& fetcher)
    : opcode{fetcher}, modrm{fetcher, opcode}, imm{fetcher, opcode, modrm} {}

template <auto T>
void FetchedInstruction::op(Cpu& cpu) const {
  return;
}

}  // namespace emu::cpu
