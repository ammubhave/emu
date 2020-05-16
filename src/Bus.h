#pragma once

#include <cstddef>
#include <cstdint>
#include <map>

class MemoryBusOperations {
 public:
  virtual ~MemoryBusOperations() = default;
  virtual uint8_t readMemory(size_t address) = 0;
  virtual void writeMemory(size_t address, uint8_t value) = 0;
};

class IOBusOperations {
 public:
  virtual ~IOBusOperations() = default;
  virtual uint8_t readPort(uint16_t port) = 0;
  virtual void writePort(uint16_t port, uint8_t value) = 0;
};

class Bus {
 public:
  uint8_t readMemory(size_t address);
  void writeMemory(size_t address, uint8_t value);
  uint8_t readPort(uint16_t port);
  void writePort(uint16_t port, uint8_t value);

  void addMemoryRange(size_t startAddress, MemoryBusOperations* device,
                      std::optional<size_t> baseAddress = std::nullopt);
  void addIoRange(size_t startAddress, IOBusOperations* device,
                  std::optional<size_t> basePort = std::nullopt);

 private:
  std::map<size_t, std::pair<MemoryBusOperations*, size_t>> memoryRange;
  std::map<uint16_t, std::pair<IOBusOperations*, uint16_t>> ioRange;
};
