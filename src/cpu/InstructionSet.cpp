#include "InstructionSet.h"

#include "cpu/ModRm.h"
#include "cpu/Opcode.h"

namespace emu::cpu {

bool hasModRm(Opcode opcode) {
  switch (opcode.po) {
    case 0xf0: {
      switch (opcode.po) {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0xd:
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1a:
        case 0x1b:
        case 0x1c:
        case 0x1d:
        case 0x1e:
        case 0x1f:
        case 0x20:
        case 0x21:
        case 0x22:
        case 0x23:
        case 0x24:
        case 0x26:
        case 0x28:
        case 0x29:
        case 0x2a:
        case 0x2b:
        case 0x2c:
        case 0x2d:
        case 0x2e:
        case 0x2f:
        case 0x38:
        case 0x3a:
        case 0x40:
        case 0x41:
        case 0x42:
        case 0x43:
        case 0x44:
        case 0x45:
        case 0x46:
        case 0x47:
        case 0x48:
        case 0x49:
        case 0x4a:
        case 0x4b:
        case 0x4c:
        case 0x4d:
        case 0x4e:
        case 0x4f:
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
        case 0x55:
        case 0x56:
        case 0x57:
        case 0x58:
        case 0x59:
        case 0x5a:
        case 0x5b:
        case 0x5c:
        case 0x5d:
        case 0x5e:
        case 0x5f:
        case 0x60:
        case 0x61:
        case 0x62:
        case 0x63:
        case 0x64:
        case 0x65:
        case 0x66:
        case 0x67:
        case 0x68:
        case 0x69:
        case 0x6a:
        case 0x6b:
        case 0x6c:
        case 0x6d:
        case 0x6e:
        case 0x6f:
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x73:
        case 0x74:
        case 0x75:
        case 0x76:
        case 0x78:
        case 0x79:
        case 0x7c:
        case 0x7d:
        case 0x7e:
        case 0x7f:
        case 0x90:
        case 0x91:
        case 0x92:
        case 0x93:
        case 0x94:
        case 0x95:
        case 0x96:
        case 0x97:
        case 0x98:
        case 0x99:
        case 0x9a:
        case 0x9b:
        case 0x9c:
        case 0x9d:
        case 0x9e:
        case 0x9f:
        case 0xa3:
        case 0xa4:
        case 0xa5:
        case 0xab:
        case 0xac:
        case 0xad:
        case 0xae:
        case 0xaf:
        case 0xb0:
        case 0xb1:
        case 0xb2:
        case 0xb3:
        case 0xb4:
        case 0xb5:
        case 0xb6:
        case 0xb7:
        case 0xba:
        case 0xbb:
        case 0xbc:
        case 0xbd:
        case 0xbe:
        case 0xbf:
        case 0xc0:
        case 0xc1:
        case 0xc2:
        case 0xc3:
        case 0xc4:
        case 0xc5:
        case 0xc6:
        case 0xc7:
        case 0xd0:
        case 0xd1:
        case 0xd2:
        case 0xd3:
        case 0xd4:
        case 0xd5:
        case 0xd6:
        case 0xd7:
        case 0xd8:
        case 0xd9:
        case 0xda:
        case 0xdb:
        case 0xdc:
        case 0xdd:
        case 0xde:
        case 0xdf:
        case 0xe0:
        case 0xe1:
        case 0xe2:
        case 0xe3:
        case 0xe4:
        case 0xe5:
        case 0xe6:
        case 0xe7:
        case 0xe8:
        case 0xe9:
        case 0xea:
        case 0xeb:
        case 0xec:
        case 0xed:
        case 0xee:
        case 0xef:
        case 0xf0:
        case 0xf1:
        case 0xf2:
        case 0xf3:
        case 0xf4:
        case 0xf5:
        case 0xf6:
        case 0xf7:
        case 0xf8:
        case 0xf9:
        case 0xfa:
        case 0xfb:
        case 0xfc:
        case 0xfd:
        case 0xfe:
          return true;
        case 0xb8:
          return opcode.pf[0] == 0xf3;
      }
      return false;
    }
    case 0x0:
    case 0x1:
    case 0x2:
    case 0x3:
    case 0x8:
    case 0x9:
    case 0xa:
    case 0xb:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x18:
    case 0x19:
    case 0x1a:
    case 0x1b:
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x23:
    case 0x28:
    case 0x29:
    case 0x2a:
    case 0x2b:
    case 0x30:
    case 0x31:
    case 0x32:
    case 0x33:
    case 0x38:
    case 0x39:
    case 0x3a:
    case 0x3b:
    case 0x62:
    case 0x63:
    case 0x69:
    case 0x6b:
    case 0x80:
    case 0x81:
    case 0x82:
    case 0x83:
    case 0x84:
    case 0x85:
    case 0x86:
    case 0x87:
    case 0x88:
    case 0x89:
    case 0x8a:
    case 0x8b:
    case 0x8c:
    case 0x8d:
    case 0x8e:
    case 0x8f:
    case 0xc0:
    case 0xc1:
    case 0xc4:
    case 0xc5:
    case 0xc6:
    case 0xc7:
    case 0xd0:
    case 0xd1:
    case 0xd2:
    case 0xd3:
    case 0xd8:
    case 0xd9:
    case 0xda:
    case 0xdb:
    case 0xdc:
    case 0xdd:
    case 0xde:
    case 0xdf:
    case 0xf6:
    case 0xf7:
    case 0xfe:
    case 0xff:
      return true;
  }
  return false;
}

size_t hasImmediate(Opcode opcode, ModRm modrm) {
  switch (opcode.po) {
    case 0xf0: {
      switch (opcode.po) {
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x73:
        case 0xa4:
        case 0xac:
        case 0xba:
        case 0xc2:
        case 0xc4:
        case 0xc5:
        case 0xc6:
          return 1;
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x86:
        case 0x87:
        case 0x88:
        case 0x89:
        case 0x8a:
        case 0x8b:
        case 0x8c:
        case 0x8d:
        case 0x8e:
        case 0x8f:
          return 2;
        case 0x3a:
          switch (opcode.so) {
            case 0x0f:
            case 0x40:
            case 0x41:
              return 0;
          }
          return 1;
      }
      return 0;
    }
    case 0x4:
    case 0xc:
    case 0x14:
    case 0x1c:
    case 0x24:
    case 0x2c:
    case 0x34:
    case 0x3c:
    case 0x6a:
    case 0x6b:
    case 0x70:
    case 0x71:
    case 0x72:
    case 0x73:
    case 0x74:
    case 0x75:
    case 0x76:
    case 0x77:
    case 0x78:
    case 0x79:
    case 0x7a:
    case 0x7b:
    case 0x7c:
    case 0x7d:
    case 0x7e:
    case 0x7f:
    case 0x80:
    case 0x82:
    case 0x83:
    case 0xa8:
    case 0xb0:
    case 0xb1:
    case 0xb2:
    case 0xb3:
    case 0xb4:
    case 0xb5:
    case 0xb6:
    case 0xb7:
    case 0xc0:
    case 0xc1:
    case 0xc6:
    case 0xcd:
    case 0xd4:
    case 0xd5:
    case 0xe0:
    case 0xe1:
    case 0xe2:
    case 0xe3:
    case 0xe4:
    case 0xe5:
    case 0xe6:
    case 0xe7:
    case 0xeb:
      return 1;
    case 0x5:
    case 0xd:
    case 0x15:
    case 0x1d:
    case 0x25:
    case 0x2d:
    case 0x35:
    case 0x3d:
    case 0x68:
    case 0x69:
    case 0x81:
    case 0xa9:
    case 0xb8:
    case 0xb9:
    case 0xba:
    case 0xbb:
    case 0xbc:
    case 0xbd:
    case 0xbe:
    case 0xbf:
    case 0xc2:
    case 0xc7:
    case 0xc8:
    case 0xca:
    case 0xe8:
    case 0xe9:
      return 2;
    case 0x9a:
    case 0xea:
      return 4;
    case 0xf6:
    case 0xf7:
      return modrm.o == 0 || modrm.o == 1;
  }
  return 0;
}

}  // namespace emu::cpu