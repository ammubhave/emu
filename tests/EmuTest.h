#pragma once

#include "Bus.h"
#include "GdbServer.h"
#include "Ram.h"
#include "Rom.h"
#include "cpu/Cpu.h"
#include "gtest/gtest.h"

class EmuTest : public ::testing::Test {
 protected:
  void SetUp() override { bus.addMemoryRange(0, &ram); }

  void setInstructionData(const std::array<uint8_t, 16>& instructionData) {
    for (size_t i = 0; i < 16; i++) {
      bus.writeMemory(0xffff0 + i, instructionData[i]);
    }
  }

  Bus bus;
  Ram<0x100000> ram;
  Cpu cpu{bus};
};
