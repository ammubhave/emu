#include "Rcl.h"

#include "cpu/operands/ImmediateOperand.h"
#include "cpu/operands/MemoryOperand.h"
#include "cpu/operands/ModRmOperand.h"
#include "cpu/operands/RegisterOperand.h"
#include "cpu/operands/RelativeOffsetOperand.h"
#include "cpu/operands/SegmentRegisterOperand.h"

namespace emu::cpu::insts {
template class Rcl<ModRmOperand<uint8_t>, ImmediateOperand<uint8_t>>;
template class Rcl<ModRmOperand<uint16_t>, ImmediateOperand<uint8_t>>;
template class Rcl<ModRmOperand<uint8_t>, RegisterOperand<uint8_t>>;
template class Rcl<ModRmOperand<uint16_t>, RegisterOperand<uint8_t>>;
}  // namespace emu::cpu::insts
