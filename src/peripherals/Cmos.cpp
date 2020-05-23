#include "Cmos.h"

#include <iostream>

namespace emu::peripherals::cmos {

Cmos::Cmos() { recomputeChecksum(); }

uint8_t Cmos::readPort(uint16_t port) {
  switch (port) {
    case 0:
      return 0;
    case 1: {
      const auto value = ram_[selectedRegister_];
      selectedRegister_ = 0xd;
      return value;
    }
  }
  __builtin_unreachable();
}

void Cmos::writePort(uint16_t port, uint8_t value) {
  switch (port) {
    case 0:
      nmiDisabled = value & 0x80;
      selectedRegister_ = value & 0x7f;
      break;
    case 1:
      ram_[selectedRegister_] = value;
      selectedRegister_ = 0xd;
      break;
    default:
      __builtin_unreachable();
  }
}

void Cmos::recomputeChecksum() {
  uint16_t checksum{0};
  for (size_t i = 0x10; i < 0x2e; i++) {
    checksum += ram_[i];
  }
  ram_[0x2e] = checksum >> 8;
  ram_[0x2f] = checksum;
}

}  // namespace emu::peripherals::cmos

// use bitflags::bitflags;

// bitflags! {
//      struct StatusRegisterB: u8 {
//         const SET    = 0b10000000;
//         const PIE    = 0b01000000;
//         const AIE    = 0b00100000;
//         const UIE    = 0b00010000;
//         const SQW    = 0b00001000;
//         const DM     = 0b00000100;
//         const HRMODE = 0b00000010;
//         const DSE    = 0b00000001;
//     }
// }

// bitflags! {
//      struct StatusRegisterC: u8 {
//         const IRQ    = 0b10000000;
//         const PF     = 0b01000000;
//         const AF     = 0b00100000;
//         const UF     = 0b00010000;
//     }
// }

// bitflags! {
//      struct StatusRegisterD: u8 {
//         const VM = 0b10000000;
//     }
// }

// pub struct Cmos {
//     selected_register: u8,
//     nmi_disable_bit: bool,

//     // status_register_a: u8,
//     // status_register_b: StatusRegisterB,
//     // status_register_c: StatusRegisterC,
//     // status_register_d: StatusRegisterD,
//     // shutdown_byte: u8,

//     // diagnostic_status_byte: u8,
//     ram: [u8; 128],
// }

// impl Cmos {
//     pub fn new() -> Self {
//         let mut ram = [0; 128];

//         // Seconds
//         ram[0x0] = 0x33;
//         // Minutes
//         ram[0x2] = 0x22;
//         // Hours
//         ram[0x4] = 0x11;
//         // Day of week
//         ram[0x6] = 1;
//         // Date of month
//         ram[0x7] = 2;
//         // Month
//         ram[0x8] = 3;
//         // Year
//         ram[0x9] = 0x85;
//         // Status Register A
//         ram[0x0a] = 0x26;
//         // Status Register B
//         ram[0x0b] = StatusRegisterB::empty().bits();
//         // Status Register C
//         ram[0x0c] = StatusRegisterC::empty().bits();
//         // Status Register D
//         ram[0x0d] = StatusRegisterD::VM.bits();
//         // Diagnostic Status Bye
//         ram[0x0e] = 0;
//         // Shutdown Byte
//         ram[0x0f] = 0;
//         // Diskette Drive Type Byte
//         ram[0x10] = 0x10;
//         // First Fixed Disk Drive Type Byte
//         ram[0x11] = 0;
//         // Equipment byte
//         ram[0x14] = 0x31;
//         // Base memory bytes
//         ram[0x15] = 0x80;
//         ram[0x16] = 0x02;

//         let mut checksum = 0u16;
//         for reg in 0x10..0x2eusize {
//             checksum = checksum.wrapping_add(u16::from(ram[reg]));
//         }
//         ram[0x2e] = (checksum >> 8) as u8;
//         ram[0x2f] = checksum as u8;

//         Cmos {
//             selected_register: 0,
//             nmi_disable_bit: false,

//             ram,
//         }
//     }
// }

// impl Peripheral for Cmos {
//     fn read_byte(&mut self, port: u16) -> u8 {
//         match port {
//             0x71 => {
//                 if self.selected_register == 0xa {
//                     self.ram[0xa] = self.ram[0xa] ^ 0x80;
//                 }

//                 let val = self.ram[usize::from(self.selected_register)];
//                 println!("Read {:x} {:x}", self.selected_register, val);
//                 val
//             }
//             _ => unimplemented!(),
//         }
//     }

//     fn write_byte(&mut self, port: u16, value: u8) {
//         match port {
//             0x70 => {
//                 self.nmi_disable_bit = value >> 7 != 0;
//                 self.selected_register = value & 0x7f;
//             }
//             0x71 => {
//                 self.ram[usize::from(self.selected_register)] = value;
//                 if self.selected_register == 0x0e {
//                     println!("SETTING CMOS DIAG TO {:x}", value);
//                 }
//             }
//             _ => unreachable!(),
//         }
//     }
// }
