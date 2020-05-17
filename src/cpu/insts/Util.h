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

#define CPL (cpu.regs_.cs.selector.rpl)
#define CS (cpu.regs_.cs.selector)
#define EIP (cpu.regs_.ip)
#define IF (cpu.regs_.flags._if)
#define IOPL (cpu.regs_.flags.iopl)
#define EX_NP(x) \
  Interrupt { Interrupt::Type::SegmentNotPresent, (x) }
#define PE (cpu.regs_.msw.pe)
#define Exception(x) throw x
#define EX_GP(x) \
  Interrupt { Interrupt::Type::GeneralProtectionFault, (x) }

#define IsWithinCodeSegmentLimits(x) \
  (!PE || !cpu.regs_.cs.accessRights.p || ((x) <= cpu.regs_.cs.segmentSize))
#define IsWithinDescriptorTableLimits(selector) \
  ((((selector.index) + 1) * 8 - 1) <=          \
   (selector.ti ? cpu.regs_.idtr.limit : cpu.regs_.gdtr.limit))
#define ReadSegmentDescriptorData(selector)                       \
  cpu.readPhysicalMemory<SegmentDescriptor>(                      \
      (selector.ti ? cpu.regs_.ldtr.base : cpu.regs_.gdtr.base) + \
      (selector.index * 8))
