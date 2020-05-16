#include "Bus.h"

uint8_t Bus::readMemory(size_t address) {
  auto it = memoryRange.upper_bound(address);
  it--;
  return it->second.first->readMemory(address - it->second.second);
}

void Bus::writeMemory(size_t address, uint8_t value) {
  auto it = memoryRange.upper_bound(address);
  it--;
  it->second.first->writeMemory(address - it->second.second, value);
}

uint8_t Bus::readPort(uint16_t port) {
  auto it = ioRange.upper_bound(port);
  it--;
  return it->second.first->readPort(port - it->second.second);
}

void Bus::writePort(uint16_t port, uint8_t value) {
  auto it = ioRange.upper_bound(port);
  it--;
  it->second.first->writePort(port - it->second.second, value);
}

void Bus::addMemoryRange(size_t startAddress, MemoryBusOperations* device,
                         std::optional<size_t> baseAddress) {
  memoryRange.try_emplace(startAddress, device,
                          baseAddress.value_or(startAddress));
}

void Bus::addIoRange(size_t startPort, IOBusOperations* device,
                     std::optional<size_t> basePort) {
  ioRange.try_emplace(startPort, device, basePort.value_or(startPort));
}
