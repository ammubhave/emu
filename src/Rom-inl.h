#include <fstream>
#include <iostream>

#include "Rom.h"

namespace {
template <size_t T>
std::array<uint8_t, T> readAllDataFromFile(std::string_view filename) {
  std::array<uint8_t, T> data;

  std::ifstream ifs{filename, std::ios::binary | std::ios::ate};
  auto pos = ifs.tellg();
  assert(pos == T);
  ifs.seekg(0, std::ios::beg);
  ifs.read(reinterpret_cast<char*>(data.data()), pos);

  return data;
}
}  // namespace

template <size_t T>
Rom<T>::Rom(std::string_view filename)
    : rom_{readAllDataFromFile<T>(filename)} {}

template <size_t T>
uint8_t Rom<T>::readMemory(size_t address) {
  // std::cout << "reading from rom " << address << std::endl;
  return rom_[address];
}

template <size_t T>
void Rom<T>::writeMemory(size_t address, uint8_t value) {
  /* no-op */
}
