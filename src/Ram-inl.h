#include "Ram.h"

template <size_t T>
uint8_t Ram<T>::readMemory(size_t address) {
  // std::cout << "reading from ram " << address << std::endl;
  return ram_[address];
}

template <size_t T>
void Ram<T>::writeMemory(size_t address, uint8_t value) {
  ram_[address] = value;
}
