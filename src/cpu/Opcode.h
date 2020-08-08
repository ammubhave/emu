#pragma once

#include <cstdint>

namespace emu::cpu {

class Cpu;
class InstructionFetcher;
// template <typename T>
// class RegisterOperand;

class Opcode {
 public:
  Opcode(InstructionFetcher fetcher);

  // template <typename T>
  // RegisterOperand<T> op_r(Cpu& cpu) const;

  uint8_t pref;
  uint8_t pf[4];
  bool of;
  uint8_t po;
  uint8_t so;
  uint8_t r;
  bool w;
  bool s;
  bool d;
  uint8_t tttn;
  uint8_t sr;
  uint8_t sre30;
  uint8_t sre33;
  uint8_t mf;
};

}  // namespace emu::cpu
