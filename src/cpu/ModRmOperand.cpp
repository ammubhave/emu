#include "ModRmOperand.h"

#include <fmt/format.h>

#include "cpu/Cpu.h"
#include "cpu/RegisterOperand.h"

namespace emu::cpu {

template <typename T>
ModRmOperand<T>::ModRmOperand(Cpu& cpu, ModRm modrm)
    : cpu_{cpu}, modrm_{modrm} {}

#define MODRM_OPERAND_OP(memop, regop)             \
  switch (modrm_.mod) {                            \
    case 0:                                        \
    case 1:                                        \
    case 2: {                                      \
      auto seg = Segment::DS;                      \
      uint16_t offset = modrm_.disp;               \
      switch (modrm_.rm) {                         \
        case 0:                                    \
          offset += cpu_.regs_.bx + cpu_.regs_.si; \
          break;                                   \
        case 1:                                    \
          offset += cpu_.regs_.bx + cpu_.regs_.di; \
          break;                                   \
        case 2:                                    \
          offset += cpu_.regs_.bp + cpu_.regs_.si; \
          seg = Segment::SS;                       \
          break;                                   \
        case 3:                                    \
          offset += cpu_.regs_.bp + cpu_.regs_.di; \
          seg = Segment::SS;                       \
          break;                                   \
        case 4:                                    \
          offset += cpu_.regs_.si;                 \
          break;                                   \
        case 5:                                    \
          offset += cpu_.regs_.si;                 \
          break;                                   \
        case 6:                                    \
          if (modrm_.mod != 0) {                   \
            offset += cpu_.regs_.bp;               \
            seg = Segment::SS;                     \
          }                                        \
          break;                                   \
        case 7:                                    \
          offset += cpu_.regs_.bx;                 \
          break;                                   \
      }                                            \
      seg = modrm_.segmentOverride.value_or(seg);  \
#memop;                                      \
    }                                              \
    case 3:                                        \
#regop;                                      \
  }                                                \
  __builtin_unreachable();

template <typename T>
T ModRmOperand<T>::r() {
  MODRM_OPERAND_OP(return MemoryOperand<T>(cpu_, seg, offset).r(),
                          return RegisterOperand<T>(cpu_, modrm_.rm).r())
}

template <typename T>
void ModRmOperand<T>::w(T v) {
  MODRM_OPERAND_OP(MemoryOperand<T>(cpu_, seg, offset).w(v),
                   RegisterOperand<T>(cpu_, modrm_.rm).w(v))
}

template <typename T>
std::string ModRmOperand<T>::str() const noexcept {
  switch (modrm_.mod) {
    case 0:
    case 1:
    case 2: {
      std::string ops;
      auto seg = Segment::DS;
      uint16_t offset = modrm_.disp;

      switch (modrm_.rm) {
        case 0:
          ops = "BX + SI";
          break;
        case 1:
          ops = "BX + DI";
          break;
        case 2:
          ops = "BP + SI";
          seg = Segment::SS;
          break;
        case 3:
          ops = "BP + DI";
          seg = Segment::SS;
          break;
        case 4:
          ops = "SI";
          break;
        case 5:
          ops = "DI";
          break;
        case 6:
          if (modrm_.mod != 0) {
            ops = "BP";
            seg = Segment::SS;
          }
          break;
        case 7:
          ops = "BX";
          break;
      }

      std::string segStr;
      switch (modrm_.segmentOverride.value_or(seg)) {
        case Segment::ES:
          segStr = "ES";
        case Segment::CS:
          segStr = "CS";
        case Segment::SS:
          segStr = "SS";
        case Segment::DS:
          segStr = "DS";
      }
      return fmt::format("{}:[{}{}0x{:x}]", segStr, ops,
                         ops == "" ? "" : ops + " + ", offset);
    }
    case 3:
      return RegisterOperand<T>(cpu_, modrm_.rm).str();
  }
  __builtin_unreachable();
}

template class ModRmOperand<uint8_t>;
template class ModRmOperand<uint16_t>;

}  // namespace emu::cpu
