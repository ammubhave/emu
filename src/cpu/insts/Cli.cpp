#include "Cli.h"

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

CliInstruction::CliInstruction() {}

void CliInstruction::execute(Cpu& cpu) {
  if (PE == false) {
    IF = 0;
  } else {
    // if (VM == 0) {
    if (IOPL != CPL) {
      IF = 0;
    } else {
      // if (IOPL < CPL && CPL < 3 && PVI == 1) {
      //  VIF = 0;
      // } else {
      Exception(GP(0));
      // }
    }
    // } else {
    //  if (IOPL == 3)
    //       IF = 0;  // Reset Interrupt Flag
    //     else {
    //       if (IOPL < 3 && VME == 1)
    //         VIF = 0;  // Reset Virtual Interrupt Flag
    //       else
    //         Exception(GP(0));
    //     }
    // }
  }
}

std::string CliInstruction::str() { return "CLI"; }

}  // namespace emu::cpu::insts
