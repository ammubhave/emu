#include "In.h"

#include "cpu/operands/ImmediateOperand.h"
#include "cpu/operands/MemoryOperand.h"
#include "cpu/operands/ModRmOperand.h"
#include "cpu/operands/RegisterOperand.h"
#include "cpu/operands/RelativeOffsetOperand.h"
#include "cpu/operands/SegmentRegisterOperand.h"

namespace emu::cpu::insts {
template class In<RegisterOperand<uint8_t>, ImmediateOperand<uint8_t>>;
template class In<RegisterOperand<uint16_t>, ImmediateOperand<uint8_t>>;
}  // namespace emu::cpu::insts
