#include "RegisterOperand.h"

#include "cpu/Cpu.h"

namespace emu::cpu {

template <typename T>
RegisterOperand<T>::RegisterOperand(Cpu& cpu, size_t idx)
    : cpu_{cpu}, idx_{idx} {}

template <typename T>
T RegisterOperand<T>::r() const noexcept {
  return cpu_.regs_.reg<T>(idx_);
}

template <typename T>
void RegisterOperand<T>::w(T v) noexcept {
  cpu_.regs_.reg<T>(idx_) = v;
}

template <>
std::string RegisterOperand<uint8_t>::str() const noexcept {
  switch (idx_) {
    case 0:
      return "AL";
    case 1:
      return "CL";
    case 2:
      return "DL";
    case 3:
      return "BL";
    case 4:
      return "AH";
    case 5:
      return "CH";
    case 6:
      return "DH";
    case 7:
      return "BH";
  }
  __builtin_unreachable();
}

template <>
std::string RegisterOperand<uint16_t>::str() const noexcept {
  switch (idx_) {
    case 0:
      return "AX";
    case 1:
      return "CX";
    case 2:
      return "DX";
    case 3:
      return "DX";
    case 4:
      return "SP";
    case 5:
      return "BP";
    case 6:
      return "SI";
    case 7:
      return "DI";
  }
  __builtin_unreachable();
}

template class RegisterOperand<uint8_t>;
template class RegisterOperand<uint16_t>;

}  // namespace emu::cpu
