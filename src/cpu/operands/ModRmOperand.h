#pragma once

#include <string>

#include "cpu/ModRm.h"

namespace emu::cpu {

class Cpu;

template <typename T>
class ModRmOperand {
 public:
  using type = T;

  ModRmOperand(Cpu& cpu, ModRm modrm);

  T r();

  void w(T v);

  std::string str() const noexcept;

 private:
  Cpu& cpu_;
  ModRm modrm_;
};

}  // namespace emu::cpu
