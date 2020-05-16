#pragma once

#include <cstdint>

enum class SystemSegmentDescriptorType {
  Invalid = 0,
  InactiveTss = 1,
  Ldt = 2,
  ActiveTss = 3,
};

enum class GateDescriptorType {
  CallGate = 4,
  TaskGate = 5,
  InterruptGate = 6,
  TrapGate = 7,
};

struct __attribute__((packed)) SegmentDescriptor {
  struct __attribute__((packed)) CodeOrDataSegmentDescriptorAccessByte {
    bool a : 1;
    bool rw : 1;
    bool ce : 1;
    bool ex : 1;
    bool s : 1;  // == 1
    uint8_t dpl : 2;
    bool p : 1;
  };
  struct __attribute__((packed)) SystemSegmentDescriptorAccessByte {
    SystemSegmentDescriptorType type : 4;
    bool s : 1;  // == 0
    uint8_t dpl : 2;
    bool p : 1;
  };

  uint16_t limit;
  uint32_t base : 24;
  union {
    struct {
      uint8_t reserved : 4;
      bool s : 1;
      uint8_t dpl : 2;
      bool p : 1;
    };
    SegmentDescriptor::CodeOrDataSegmentDescriptorAccessByte data;
    SegmentDescriptor::SystemSegmentDescriptorAccessByte system;
  } accessByte;
  uint16_t reserved;
};
static_assert(sizeof(SegmentDescriptor) == 8);

union SegmentSelector {
  uint16_t value;
  struct __attribute__((packed)) {
    uint8_t rpl : 2;
    bool ti : 1;
    uint16_t index : 13;
  };
};
static_assert(sizeof(SegmentSelector) == 2);

struct __attribute__((packed)) ErrorCode {
  bool ext : 1;
  bool idt : 1;
  bool ti : 1;  // 1=LDT 0=GDT
  uint16_t index : 13;
};
static_assert(sizeof(ErrorCode) == 2);

struct __attribute__((packed)) GateDescriptor {
  uint16_t offset;
  SegmentSelector selector;
  uint8_t wordCount : 5;
  uint8_t reserved1 : 3;
  GateDescriptorType type : 4;
  bool reserved2 : 1;  // 0
  uint8_t dpl : 2;
  bool p : 1;
  uint16_t reserved;
};
static_assert(sizeof(GateDescriptor) == 8);

struct Interrupt {
  enum class Type : uint8_t {
    DivideErrorException = 0,
    SingleStepInterrupt,
    NmiInterrupt,
    BreakpointInterrupt,
    IntoDetectedOverflowException,
    BoundRangeExceededException,
    InvalidOpcodeException,
    ProcessorExtentionNotAvailableException,
    DoubleExceptionDetected,
    ProcessorExtentionSegmentOverrunInterrupt,
    InvalidTaskStateSegment,
    SegmentNotPresent,
    StackSegmentOverrunOrNotPresent,
    GeneralProtectionFault,

    InterruptTableLimitTooSmall = 8,
    SegmentOverrunException = 13,
    ProcessorExtensionErrorInterrupt = 16,
  };

  union {
    Type type;
    uint8_t number;
  };

  ErrorCode errorCode;
};
static_assert(sizeof(Interrupt) == 3);

// enum class Segment { CS, DS, SS, ES };
enum class MemoryOperationType { Read, Write, Execute };
