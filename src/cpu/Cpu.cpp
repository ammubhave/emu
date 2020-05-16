#include "Cpu.h"

#include <fmt/format.h>

#include <array>
#include <iostream>

#include "cpu/Immediate.h"
#include "cpu/ImmediateOperand.h"
#include "cpu/InstructionFetcher.h"
#include "cpu/ModRm.h"
#include "cpu/ModRmOperand.h"
#include "cpu/Opcode.h"
#include "cpu/RegisterOperand.h"
#include "cpu/SegmentRegisterOperand.h"
#include "cpu/Types.h"
#include "cpu/insts/AllInstructions.h"
#include "cpu/insts/Instruction.h"

using namespace emu::cpu::insts;

namespace emu::cpu {

Cpu::Cpu(Bus& bus) noexcept : bus_{bus} {}

void Cpu::step() noexcept {
  InstructionFetcher fetcher{*this};

  try {
    // Fetch Instruction
    size_t iaddr = paddr(Segment::CS, regs_.ip, MemoryOperationType::Execute);
    Opcode opcode{fetcher};
    ModRm modrm{fetcher, opcode};
    Immediate imm{fetcher, opcode, modrm};

    // Decode Instruction
    std::unique_ptr<Instruction> inst;
    switch (opcode.po) {
      case 0xb8:
      case 0xb9:
      case 0xba:
      case 0xbb:
      case 0xbc:
      case 0xbd:
      case 0xbe:
      case 0xbf:
        inst = std::make_unique<MovInstruction<RegisterOperand<uint16_t>,
                                               ImmediateOperand<uint16_t>>>(
            opcode.op_r<uint16_t>(*this), imm.op_v());
        break;
      case 0xe9:
        inst = std::make_unique<JmpNearRelativeInstruction>(imm.v());
        break;
      case 0xea:
        inst = std::make_unique<JmpFarAbsoluteDirectInstruction>(imm.Asel(),
                                                                 imm.Aoff());
        break;
      case 0xfa:
        inst = std::make_unique<CliInstruction>();
        break;
      default:
        std::cout << fmt::format("0x{:x}: Unknown opcode {:x}\n", iaddr,
                                 opcode.po);
    }

    std::cout << fmt::format("0x{:x}: {}\n", iaddr, inst->str());

    // Execute instruction
    inst->execute(*this);
  } catch (Interrupt interrupt) {
    try {
      std::cerr << interrupt.number << std::endl;
      exit(1);
    } catch (Interrupt interrupt) {
      // double fault
    }
  }
}

template <typename T>
T Cpu::readPhysicalMemory(size_t addr) {
  T v{0};
  for (size_t i = 0; i < sizeof(T); ++i) {
    v |= bus_.readMemory(addr + i) << (i * 8);
  }
  return v;
}
template uint8_t Cpu::readPhysicalMemory(size_t);
template uint16_t Cpu::readPhysicalMemory(size_t);
template uint32_t Cpu::readPhysicalMemory(size_t);

template <typename T>
void Cpu::writePhysicalMemory(size_t addr, T v) {
  for (size_t i = 0; i < sizeof(T); ++i) {
    bus_.writeMemory(addr++, static_cast<uint8_t>(v));
    v >>= 8;
  }
}
template void Cpu::writePhysicalMemory(size_t, uint8_t);
template void Cpu::writePhysicalMemory(size_t, uint16_t);
template void Cpu::writePhysicalMemory(size_t, uint32_t);

template <typename T>
T Cpu::readLogicalMemory(Segment seg, uint16_t offset) {
  // TODO: if real mode and offset is 0xffff and reading a word, throw fault

  std::array<size_t, sizeof(T)> addrs;
  for (size_t i = 0; i < sizeof(T); ++i) {
    addrs[i] = paddr(seg, offset + i, MemoryOperationType::Read);
  }
  T v{0};
  for (size_t i = 0; i < sizeof(T); ++i) {
    v |= readPhysicalMemory<uint8_t>(addrs[i]) << (i * 8);
  }
  return v;
}
template uint8_t Cpu::readLogicalMemory(Segment, uint16_t);
template uint16_t Cpu::readLogicalMemory(Segment, uint16_t);
template uint32_t Cpu::readLogicalMemory(Segment, uint16_t);

template <typename T>
void Cpu::writeLogicalMemory(Segment seg, uint16_t offset, T v) {
  // TODO: if real mode and offset is 0xffff and reading a word, throw fault

  std::array<size_t, sizeof(T)> addrs;
  for (size_t i = 0; i < sizeof(T); ++i) {
    addrs[i] = paddr(seg, offset + i, MemoryOperationType::Write);
  }
  for (size_t i = 0; i < sizeof(T); ++i) {
    writePhysicalMemory<uint8_t>(addrs[i], static_cast<uint8_t>(v));
    v >>= 8;
  }
}
template void Cpu::writeLogicalMemory(Segment, uint16_t, uint8_t);
template void Cpu::writeLogicalMemory(Segment, uint16_t, uint16_t);
template void Cpu::writeLogicalMemory(Segment, uint16_t, uint32_t);

size_t Cpu::instructionFetchAddress() const {
  return paddr(Segment::CS, regs_.ip, MemoryOperationType::Execute);
}

size_t Cpu::paddr(Segment segment, uint16_t offset,
                  MemoryOperationType memoryOperationType) const {
  if (regs_.msw.pe) {
    return 0;
  } else {
    if (offset > 0xffff) {
      if (segment == Segment::SS)
        throw Interrupt{{Interrupt::Type::StackSegmentOverrunOrNotPresent}, {}};
      else
        throw Interrupt{{Interrupt::Type::GeneralProtectionFault}, {}};
    }
    return ((size_t)regs_.segmentRegisters[(size_t)segment] << 4) + offset;
  }
}

}  // namespace emu::cpu
