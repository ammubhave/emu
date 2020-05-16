#pragma once

#include <assert.h>
#include <stddef.h>

#include <cstdint>

#include "Types.h"

// struct __attribute__((packed)) CodeSegmentRegister {
//   uint16_t segmentSize;
//   uint32_t baseAddress : 24;
//   SegmentDescriptor::DataSegmentDescriptorAccessByte accessRights;
//   SegmentSelector selector;
// };
// static_assert(sizeof(CodeSegmentRegister) == 8);

struct __attribute__((packed)) CodeOrDataSegmentRegister {
  uint16_t segmentSize;
  uint32_t baseAddress : 24;
  SegmentDescriptor::CodeOrDataSegmentDescriptorAccessByte accessRights;
  SegmentSelector selector;
};
static_assert(sizeof(CodeOrDataSegmentRegister) == 8);

struct __attribute__((packed)) DescriptorTableRegister {
  uint16_t limit;
  uint32_t base : 24;
};
static_assert(sizeof(DescriptorTableRegister) == 5);

struct __attribute__((packed)) WithSelectorDescriptorTableRegister {
  uint16_t limit;
  uint32_t base : 24;
  SegmentSelector selector;
};
static_assert(sizeof(WithSelectorDescriptorTableRegister) == 7);

struct Registers {
  // General Purpose Registers
  union {
    struct {
      union {
        struct {
          uint8_t al;
          uint8_t ah;
        };
        uint16_t ax;
      };
      union {
        struct {
          uint8_t cl;
          uint8_t ch;
        };
        uint16_t cx;
      };
      union {
        struct {
          uint8_t dl;
          uint8_t dh;
        };
        uint16_t dx;
      };
      union {
        struct {
          uint8_t bl;
          uint8_t bh;
        };
        uint16_t bx;
      };

      uint16_t sp;
      uint16_t bp;
      uint16_t si;
      uint16_t di;
    };

    uint16_t generalPurposeRegisters[8];
    uint8_t generalPurposeRegisters8[8];
  };
  template <typename T>
  T& reg(size_t i);

  // Memory Management Registers
  union {
    struct {
      uint16_t es;
      uint16_t cs;
      uint16_t ss;
      uint16_t ds;
    };
    uint16_t segmentRegisters[4]{0, 0xf000, 0, 0};
  };
  uint16_t& sreg(size_t i);

  //   CodeSegmentRegister cs{0xffff, 0xff0000, {}, {0xf000}};
  //   DataSegmentRegister ds{0xffff, 0, {}, {0}};
  //   DataSegmentRegister ss{0, 0, {}, {0}};
  //   DataSegmentRegister es{0xffff, 0, {}, {0}};

  DescriptorTableRegister gdtr;
  DescriptorTableRegister idtr{0x3ff, 0};
  WithSelectorDescriptorTableRegister ldtr;

  // tss
  WithSelectorDescriptorTableRegister tr;

  // Status and Control Registers
  union Flags {
    uint16_t value;
    struct {
      bool cf : 1;
      bool reserved1 : 1;
      bool pf : 1;
      bool reserved2 : 1;
      bool af : 1;
      bool reserved3 : 1;
      bool zf : 1;
      bool sf : 1;
      bool tf : 1;
      bool _if : 1;
      bool df : 1;
      bool of : 1;
      uint8_t iopl : 2;
      bool nt : 1;
      bool reserved4 : 1;
    };
  } flags{0x2};
  static_assert(sizeof(flags) == 2);

  uint16_t ip{0xfff0};

  struct __attribute__((packed)) Msw {
    bool pe : 1;
    bool mp : 1;
    bool em : 1;
    bool ts : 1;
    uint16_t reserved : 12;
  } msw{false, false, false, false, 0xfff};
  static_assert(sizeof(msw) == 2);
};
