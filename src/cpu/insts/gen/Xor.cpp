#include "Xor.h"

#include "cpu/operands/ImmediateOperand.h"
#include "cpu/operands/MemoryOperand.h"
#include "cpu/operands/ModRmOperand.h"
#include "cpu/operands/RegisterOperand.h"
#include "cpu/operands/RelativeOffsetOperand.h"
#include "cpu/operands/SegmentRegisterOperand.h"

namespace emu::cpu::insts {
template class Xor<ModRmOperand<uint8_t>, RegisterOperand<uint8_t>>;
template class Xor<ModRmOperand<uint16_t>, RegisterOperand<uint16_t>>;
template class Xor<RegisterOperand<uint8_t>, ModRmOperand<uint8_t>>;
template class Xor<RegisterOperand<uint16_t>, ModRmOperand<uint16_t>>;
template class Xor<RegisterOperand<uint8_t>, ImmediateOperand<uint8_t>>;
template class Xor<RegisterOperand<uint16_t>, ImmediateOperand<uint16_t>>;
}  // namespace emu::cpu::insts
