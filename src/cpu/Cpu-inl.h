#include "Cpu.h"

namespace emu::cpu {

template <typename T>
T Cpu::readPhysicalMemory(size_t addr) {
  T v;
  std::array<uint8_t, sizeof(T)> data;
  for (size_t i = 0; i < sizeof(T); ++i) {
    data[i] = bus_.readMemory(addr + i);
  }
  std::memcpy(&v, data.data(), data.size());
  return v;
}

template <typename T>
void Cpu::writePhysicalMemory(size_t addr, T v) {
  for (size_t i = 0; i < sizeof(T); ++i) {
    bus_.writeMemory(addr++, static_cast<uint8_t>(v));
    v >>= 8;
  }
}

template <typename T>
T Cpu::readLogicalMemory(Segment seg, uint16_t offset) {
  // TODO: if real mode and offset is 0xffff and reading a word, throw fault

  std::array<size_t, sizeof(T)> addrs;
  for (size_t i = 0; i < sizeof(T); ++i) {
    addrs[i] = paddr(seg, offset + i, MemoryOperationType::Read);
  }

  T v;
  std::array<uint8_t, sizeof(T)> data;
  for (size_t i = 0; i < sizeof(T); ++i) {
    data[i] = readPhysicalMemory<uint8_t>(addrs[i]);
  }
  std::memcpy(&v, data.data(), data.size());
  return v;
}

template <typename T>
void Cpu::writeLogicalMemory(Segment seg, uint16_t offset, T v) {
  // TODO: if real mode and offset is 0xffff and writing a word, throw fault

  std::array<size_t, sizeof(T)> addrs;
  for (size_t i = 0; i < sizeof(T); ++i) {
    addrs[i] = paddr(seg, offset + i, MemoryOperationType::Write);
  }

  std::array<uint8_t, sizeof(T)> data;
  std::memcpy(data.data(), &v, data.size());
  for (size_t i = 0; i < data.size(); i++) {
    writePhysicalMemory<uint8_t>(addrs[i], data[i]);
  }
}

}  // namespace emu::cpu
