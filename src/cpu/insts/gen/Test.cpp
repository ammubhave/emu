#include "Test.h"

#include "cpu/operands/ImmediateOperand.h"
#include "cpu/operands/MemoryOperand.h"
#include "cpu/operands/ModRmOperand.h"
#include "cpu/operands/RegisterOperand.h"
#include "cpu/operands/RelativeOffsetOperand.h"
#include "cpu/operands/SegmentRegisterOperand.h"

namespace emu::cpu::insts {
template class Test<RegisterOperand<uint8_t>, ImmediateOperand<uint8_t>>;
template class Test<RegisterOperand<uint16_t>, ImmediateOperand<uint16_t>>;
}  // namespace emu::cpu::insts
