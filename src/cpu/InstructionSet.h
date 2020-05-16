#pragma once

#include <cstddef>

namespace emu::cpu {

class ModRm;
class Opcode;

bool hasModRm(Opcode opcode);
size_t hasImmediate(Opcode opcode, ModRm modrm);

}  // namespace emu::cpu
