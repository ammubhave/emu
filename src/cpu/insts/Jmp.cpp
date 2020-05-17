#include "Jmp.h"

#include <fmt/format.h>

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

template <bool IsNearJump, bool IsRelativeJump, bool IsDirectJump>
JmpInstruction<IsNearJump, IsRelativeJump, IsDirectJump>::JmpInstruction(
    Operand dst)
    : dst_{dst} {}

template <bool IsNearJump, bool IsRelativeJump, bool IsDirectJump>
void JmpInstruction<IsNearJump, IsRelativeJump, IsDirectJump>::execute(
    Cpu& cpu) {
  uint16_t /* uint32_t */ temporaryEIP = 0;
  if constexpr (IsNearJump) {
    const auto destination = ([&]() {
      if constexpr (IsDirectJump) {
        return dst_;
      } else {
        return dst_.r();
      }
    })();

    if constexpr (IsRelativeJump) {
      temporaryEIP = EIP + destination;
    } else {
      temporaryEIP = destination;
    }
    if (!IsWithinCodeSegmentLimits(temporaryEIP)) {
      Exception(EX_GP(0));
    }
    // if(OperandSize == 32) {
    //     EIP = TemporaryEIP;
    // } else {
    EIP = temporaryEIP /* & 0xFFFF */;
    // }
  } else {
    const auto v = ([&]() {
      if constexpr (IsDirectJump) {
        return dst_;
      } else {
        return dst_.r();
      }
    })();

    if (PE == false) {
      struct {
        uint16_t offset;
        uint16_t segmentSelector;
      } destination{static_cast<uint16_t>(v), static_cast<uint16_t>(v >> 16)};

      uint16_t temporaryEIP = destination.offset;
      if (!IsWithinCodeSegmentLimits(temporaryEIP)) {
        Exception(EX_GP(0));
      }
      CS = destination.segmentSelector;
      // if (OperandSize == 32) {
      //   EIP = TemporaryEIP;
      // } else {
      EIP = temporaryEIP /* & 0xFFFF */;
      // }
    } else {
      struct {
        uint16_t offset;
        SegmentSelector segmentSelector;
      } destination{static_cast<uint16_t>(v), static_cast<uint16_t>(v >> 16)};

      if (destination.segmentSelector.value == 0) {
        Exception(EX_GP(0));
      }
      if (!IsWithinDescriptorTableLimits(destination.segmentSelector.index)) {
        Exception(EX_GP(destination.segmentSelector.value));
      }
      auto segmentDescriptorData =
          ReadSegmentDescriptorData(destination.segmentSelector);
      const auto DPL = segmentDescriptorData.accessByte.dpl;

      if (segmentDescriptorData.isTypeConformingCodeSegment()) {
        if (DPL > CPL) {
          Exception(EX_GP(segmentSelector.value));
        }
        if (!segmentDescriptorData.isPresent()) {
          Exception(EX_NP(SegmentSelector));
        }
        temporaryEIP = destination.offset;
        // if (OperandSize == 16) TemporaryEIP = TemporaryEIP & 0xFFFF;
        if (!IsWithinCodeSegmentLimits(TemporaryEIP)) {
          Exception(EX_GP(0));
        }
        CS = destination.segmentSelector.value;
        CS.rpl = CPL;
        EIP = TemporaryEIP;
        break;

      } else if (segmentDescriptorData.isTypeNonConformingCodeSegment()) {
        if (RPL > CPL || DPL != CPL) Exception(GP(CodeSegmentSelector));
        if (!IsPresent(Segment)) Exception(NP(SegmentSelector));
        if (!IsWithinCodeSegmentLimits(InstructionPointer)) Exception(GP(0));
        TemporaryEIP = Destination.Offset;
        if (OperandSize == 16) TemporaryEIP = TemporaryEIP & 0xFFFF;
        if (!IsWithinCodeSegmentLimits(TemporaryEIP)) Exception(GP(0));
        CS =
            Destination
                .SegmentSelector;  // segment descriptor information also loaded
        CS.RPL = CPL;
        EIP = TemporaryEIP;
        break;
      } else if (segmentDescriptorData.isTypeCallGate()) {
        if (CallGate.DPL < CPL || CallGate.DPL < CallGate.SegmentSelector.RPL)
          Exception(GP(CallGate.Selector));
        if (!IsPresent(CallGate)) Exception(NP(CallGate.Selector));
        if (CallGate.CodeSegmentSelector == 0) Exception(GP(0));
        CodeSegmentDescriptor = ReadCodeSegmentDescriptor();
        if (!IndicatesCodeSegment(CodeSegmentDescriptor) ||
            (IsConforming(CodeSegmentDescriptor) && DPL > CPL) ||
            (!IsConforming(CodeSegmentDescriptor) && DPL != CPL))
          Exception(GP(CodeSegmentSelector));
        if (!IsPresent(CodeSegment)) Exception(NP(CodeSegmentSelector));
        if (!IsWithinCodeSegmentLimits(InstructionPointer)) Exception(GP(0));
        TemporaryEIP = Destination.Offset;
        if (GateSize == 16) TemporaryEIP = TemporaryEIP & 0xFFFF;
        if (!IsWithinCodeSegmentLimits(TemporaryEIP)) Exception(GP(0));
        CS =
            Destination
                .SegmentSelector;  // segment descriptor information also loaded
        CS.RPL = CPL;
        EIP = TemporaryEIP;
        break;
      } else if (segmentDescriptorData.isTypeTaskGate()) {
        if (TaskGate.DPL < CPL || TaskGate.DPL < TaskGate.SegmentSelector.RPL)
          Exception(GP(TaskGate.Selector));
        if (!IsPresent(TaskGate)) Exception(NP(TaskGate.Selector));
        TSSSegmentSelector = ReadTSSSegmentSelector(
            TaskGateDescriptor);  // Read the TSS segment selector in the
                                  // task-gate descriptor;
        if (!IsGlobal(TSSSegmentSelector) /*TSS segment selector local/global
                                             bit is set to local*/
            || !IsWithinGDTLimits(Index) || TSSIsBusy(TSSDescriptor))
          Exception(GP(TSSSelector));
        if (!IsPresent(TSS)) Exception(NP(TSSSelector));
        SwitchTasks(TSS);  // Switch tasks to TSS
        if (!IsWithinCodeSegmentLimit(EIP)) Exception(GP(0));
        break;
      } else if (segmentDescriptorData.isTypeTaskStateSegment()) {
        if (TSS.DPL < CPL ||
            TSS.DPL < TSS.SegmentSelector.RPL || !TSSIsAvailable(TSSDescriptor) /*TSS descriptor indicates that TSS is not available*/)
          Exception(GP(TSSSelector));
        if (!IsPresent(TSS)) Exception(NP(TSSSelector));
        SwitchTasks(TSS);  // Switch tasks to TSS
        if (!IsWithinCodeSegmentLimit(EIP)) Exception(GP(0));
        break;
      } else {
        Exception(GP(SegmentSelector))
      }
    }
  }
}

template <bool IsNearJump, bool IsRelativeJump, bool IsDirectJump>
std::string JmpInstruction<IsNearJump, IsRelativeJump, IsDirectJump>::str() {
  return fmt::format(
      "JMP{}\t{}{}{}", IsNearJump ? "" : "F", IsRelativeJump ? "IP + " : "",
      ([&]() {
        if constexpr (IsDirectJump && !IsNearJump) {
          return fmt::format("0x{:x}:", dst_ >> 16);
        } else {
          return "";
        }
      })(),
      ([&]() {
        if constexpr (IsDirectJump) {
          return fmt::format("0x{:x}", static_cast<uint16_t>(dst_));
        } else {
          return IsNearJump ? dst_.str() : fmt::format("*{}", dst_.str());
        }
      })());
}

template class JmpInstruction<true, true, true>;
template class JmpInstruction<true, false, false>;
template class JmpInstruction<false, false, true>;
template class JmpInstruction<false, false, false>;

}  // namespace emu::cpu::insts
