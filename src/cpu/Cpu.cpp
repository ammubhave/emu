#include "Cpu.h"

#include <fmt/format.h>

#include <array>
#include <iostream>

#include "cpu/Immediate.h"
#include "cpu/InstructionDecoder.h"
#include "cpu/InstructionFetcher.h"
#include "cpu/ModRm.h"
#include "cpu/Opcode.h"
#include "cpu/Types.h"
#include "cpu/insts/AllInstructions.h"
#include "cpu/insts/Instruction.h"
#include "cpu/operands/ImmediateOperand.h"
#include "cpu/operands/ModRmOperand.h"
#include "cpu/operands/RegisterOperand.h"
#include "cpu/operands/SegmentRegisterOperand.h"

using namespace emu::cpu::insts;

namespace emu::cpu {

Cpu::Cpu(Bus& bus) noexcept : bus_{bus} {}

void Cpu::step() noexcept {
  try {
    // Fetch Instruction
    size_t iaddr = paddr(Segment::CS, regs_.ip, MemoryOperationType::Execute);

    const auto fetchedInstruction =
        InstructionFetcher{*this}.fetchNextInstruction();
    const auto& opcode = fetchedInstruction.opcode;
    const auto& modrm = fetchedInstruction.modrm;
    const auto& imm = fetchedInstruction.imm;

    // Decode Instruction
    const auto decodedInstruction =
        InstructionDecoder{*this}.decode(fetchedInstruction);

    // std::unique_ptr<Instruction> inst;
    // switch (opcode.po) {
    //   case 0xb8:
    //   case 0xb9:
    //   case 0xba:
    //   case 0xbb:
    //   case 0xbc:
    //   case 0xbd:
    //   case 0xbe:
    //   case 0xbf:
    //     inst = std::make_unique<
    //         Mov<RegisterOperand<uint16_t>, ImmediateOperand<uint16_t>>>(
    //         opcode.op_r<uint16_t>(*this), imm.op_v());
    //     break;
    //   case 0xe9:
    //     inst = std::make_unique<Jmp<true, true, true>>(imm.v());
    //     break;
    //   case 0xea:
    //     inst = std::make_unique<Jmp<false, false, true>>(imm.A());
    //     break;
    //   case 0xfa:
    //     inst = std::make_unique<Cli>();
    //     break;
    //   default:
    //     if (debugEnabled_) {
    //       std::cout << fmt::format("0x{:x}: Unknown opcode {:x}\n",
    //       iaddr,
    //                                opcode.po);
    //     }
    // }

    if (debugEnabled_) {
      std::cout << fmt::format("0x{:x}: {}\n", iaddr,
                               decodedInstruction->str());
    }

    // Execute instruction
    decodedInstruction->execute(*this);
  } catch (Interrupt interrupt) {
    try {
      if (debugEnabled_) {
        std::cerr << "Interrupt: " << (int)interrupt.number << std::endl;
      }
      exit(1);
    } catch (Interrupt interrupt) {
      // double fault
    }
  }
}

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
        throw Interrupt{Interrupt::Type::StackSegmentOverrunOrNotPresent};
      else
        throw Interrupt{Interrupt::Type::GeneralProtectionFault};
    }
    return ((size_t)regs_.segmentRegisters[segment.index()].selector.value
            << 4) +
           offset;
  }
}

}  // namespace emu::cpu
