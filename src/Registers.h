#pragma once

#include <assert.h>
#include <stddef.h>

#include <array>
#include <cstdint>

#include "Types.h"

struct __attribute__((packed)) CodeOrDataSegmentRegister {
  CodeOrDataSegmentRegister(uint16_t value)
      : segmentSize{}, baseAddress{}, accessRights{}, selector{} {
    selector.value = value;
  }

  operator uint8_t() const { return selector.value; }
  CodeOrDataSegmentRegister& operator=(uint8_t value) noexcept {
    selector.value = value;
    return *this;
  }

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

    std::array<uint16_t, 8> generalPurposeRegisters;
    std::array<uint8_t, 8> generalPurposeRegisters8;
  };
  template <typename T>
  T& reg(size_t i);

  // Memory Management Registers
  union {
    struct {
      CodeOrDataSegmentRegister es;
      CodeOrDataSegmentRegister cs;
      CodeOrDataSegmentRegister ss;
      CodeOrDataSegmentRegister ds;
    };
    CodeOrDataSegmentRegister segmentRegisters[4]{0, 0xf000, 0, 0};
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
  Flags flags{};
  uint16_t ip{0xfff0};
  Cr0 cr0{};
  uint64_t cr2{};
  Cr3 cr3{};
  Cr4 cr4{};
  Ia32Efer is32efer{};
};

union Flags {
  Flags() : value{0x2} {}
  Flags(uint64_t v) : value{v} {}
  Flags& operator=(uint64_t v) {
    value = v;
    return *this;
  }
  operator uint64_t() const { return value; }

  uint64_t value;
  struct __attribute__((packed)) {
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
    bool rf : 1;
    bool vm : 1;
    bool ac : 1;
    bool vif : 1;
    bool vip : 1;
    bool id : 1;
    uint64_t reserved5 : 42;
  };
};
static_assert(sizeof(Flags) == sizeof(uint64_t));

union Cr0 {
  Cr0() : value{} {}
  Cr0(uint64_t v) : value{v} {}
  Cr0& operator=(uint64_t v) {
    value = v;
    return *this;
  }
  operator uint64_t() const { return value; }

  uint64_t value;
  struct __attribute__((packed)) {
    bool pe : 1;
    bool mp : 1;
    bool em : 1;
    bool ts : 1;
    bool et : 1;
    bool ne : 1;
    uint16_t reserved1 : 10;
    bool wp : 1;
    bool reserved2 : 1;
    bool am : 1;
    uint16_t reserved3 : 10;
    bool nw : 1;
    bool cd : 1;
    bool pg : 1;
    uint32_t reserved4 : 32;
  };
};
static_assert(sizeof(Cr0) == sizeof(uint64_t));

union Cr3 {
  Cr3() : value{} {}
  Cr3(uint64_t v) : value{v} {}
  Cr3& operator=(uint64_t v) {
    value = v;
    return *this;
  }
  operator uint64_t() const { return value; }

  uint64_t value;
  uint16_t pcid : 12;
  struct __attribute__((packed)) {
    uint8_t reserved1 : 3;
    bool pwt : 1;
    bool pcd : 1;
    uint8_t reserved2 : 7;
    uint64_t pageDirectoryBase : 52;
  };
};
static_assert(sizeof(Cr3) == sizeof(uint64_t));

union Cr4 {
  Cr4() : value{} {}
  Cr4(uint64_t v) : value{v} {}
  Cr4& operator=(uint64_t v) {
    value = v;
    return *this;
  }
  operator uint64_t() const { return value; }

  uint64_t value;
  struct __attribute__((packed)) {
    bool vme : 1;
    bool pvi : 1;
    bool tsd : 1;
    bool de : 1;
    bool pse : 1;
    bool pae : 1;
    bool mce : 1;
    bool pge : 1;
    bool pce : 1;
    bool osfxsr : 1;
    bool osxmmexcpt : 1;
    bool umip : 1;
    bool reserved1 : 1;
    bool vmxe : 1;
    bool smxe : 1;
    bool reserved2 : 1;
    bool pcide : 1;
    bool osxsave : 1;
    bool reserved3 : 1;
    bool smep : 1;
    bool smap : 1;
    uint64_t reserved4 : 42;
  };
};
static_assert(sizeof(Cr4) == sizeof(uint64_t));

union Cr8 {
  Cr8() : value{} {}
  Cr8(uint64_t v) : value{v} {}
  Cr8& operator=(uint64_t v) {
    value = v;
    return *this;
  }
  operator uint64_t() const { return value; }

  uint64_t value;
  uint8_t priority : 4;
};
static_assert(sizeof(Cr3) == sizeof(uint64_t));

union Ia32Efer {
  Ia32Efer() : value{} {}
  Ia32Efer(uint64_t v) : value{v} {}
  Ia32Efer& operator=(uint64_t v) {
    value = v;
    return *this;
  }
  operator uint64_t() const { return value; }

  uint64_t value;
  struct __attribute__((packed)) {
    bool sce : 1;
    uint8_t reserved1 : 7;
    bool lme : 1;
    bool reserved2 : 1;
    bool lma : 1;

    bool nxe : 1;
    bool svme : 1;
    bool lmsle : 1;
    bool ffxsr : 1;
    bool tce : 1;
    uint64_t reserved4 : 48;
  };
};
