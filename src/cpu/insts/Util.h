// if(PE == 0) IF = 0; //Reset Interrupt Flag
// else {
// 	if(VM == 0) {
// 		if(IOPL != CPL) IF = 0; //Reset Interrupt Flag
// 		else {
// 			if(IOPL < CPL && CPL < 3 && PVI == 1) VIF = 0; //Reset
// Virtual Interrupt Flag 			else Exception(GP(0));
// 		}
// 	}
// 	else {
// 		if(IOPL == 3) IF = 0; //Reset Interrupt Flag
// 		else {
// 			if(IOPL < 3 && VME == 1) VIF = 0; //Reset Virtual
// Interrupt Flag 			else Exception(GP(0));
// 		}
// 	}
// }

#pragma once

#define AF (cpu.regs_.flags.af)
#define CF (cpu.regs_.flags.cf)
#define DF (cpu.regs_.flags.df)
#define IF (cpu.regs_.flags._if)
#define IOPL (cpu.regs_.flags.iopl)
#define OF (cpu.regs_.flags.of)
#define PF (cpu.regs_.flags.pf)
#define SF (cpu.regs_.flags.sf)
#define ZF (cpu.regs_.flags.zf)

#define CPL (cpu.regs_.cs.selector.rpl)
#define CS (cpu.regs_.cs.selector)
#define IP (cpu.regs_.ip)
#define EX_NP(x) \
  Interrupt { Interrupt::Type::SegmentNotPresent, (x) }
#define CR0 (cpu.regs_.msw)
#define PE pe
#define Exception(x) throw x
#define EX_GP(x) \
  Interrupt { Interrupt::Type::GeneralProtectionFault, (x) }

#define IsWithinCodeSegmentLimits(x) \
  (!CR0.PE || !cpu.regs_.cs.accessRights.p || ((x) <= cpu.regs_.cs.segmentSize))
#define IsWithinDescriptorTableLimits(selector) \
  ((((selector.index) + 1) * 8 - 1) <=          \
   (selector.ti ? cpu.regs_.idtr.limit : cpu.regs_.gdtr.limit))
#define ReadSegmentDescriptorData(selector)                       \
  cpu.readPhysicalMemory<SegmentDescriptor>(                      \
      (selector.ti ? cpu.regs_.ldtr.base : cpu.regs_.gdtr.base) + \
      (selector.index * 8))

template <typename T>
bool lsb(T v) {
  return (v & 1) != 0;
}

template <typename T>
bool msb(T v) {
  return (v & (static_cast<T>(1) << (sizeof(T) * 8 - 1))) != 0;
}

template <typename T>
bool calc_sf(T v) {
  return msb(v);
}
template <typename T>
bool calc_zf(T v) {
  return v == 0;
}

template <typename T>
bool calc_pf(T v) {
  return __builtin_popcount(static_cast<uint8_t>(v)) % 2 == 0;
}

template <typename Cpu, typename Op>
void Jcc(Cpu& cpu, Op op, std::function<bool()> fn) {
  if (fn()) {
    IP = op.r();
  }
}

template <bool WriteToDest, typename Cpu, typename Op1, typename Op2>
void arith(Cpu& cpu, Op1 op1, Op2 op2,
           std::function<std::tuple<typename Op1::type, bool, bool, bool>(
               typename Op1::type a, uint16_t b, bool cf)>
               op) {
  const auto [result, cf, of, af] = op(op1.r(), op2.r(), CF);

  if constexpr (WriteToDest) {
    op1.w(result);
  }

  CF = cf;
  OF = of;
  AF = af;
  SF = calc_sf(result);
  ZF = calc_zf(result);
  PF = calc_pf(result);
}

template <bool IsShift, typename Cpu, typename Op1, typename Op2>
void rotate_shift(Cpu& cpu, Op1 op1, Op2 op2,
                  std::function<std::tuple<typename Op1::type, bool, bool>(
                      typename Op1::type a, uint16_t b, bool cf)>
                      op) {
  const uint16_t count = op2.r() & 0x1f;
  if (count == 0) return;

  const auto [result, cf, of] = op(op1.r(), count, CF);
  op1.w(result);
  CF = cf;
  OF = of;
  if constexpr (IsShift) {
    SF = calc_sf(result);
    ZF = calc_zf(result);
    PF = calc_pf(result);
  }
}
