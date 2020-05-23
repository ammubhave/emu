#include "gen/Shl.h"

#include <iostream>

#include "cpu/Cpu.h"
#include "cpu/insts/Util.h"

namespace emu::cpu::insts {

template <typename Op1, typename Op2>
void Shl<Op1, Op2>::execute(Cpu& cpu) {
  rotate_shift<true>(
      cpu, op1_, op2_,
      [&](typename Op1::type a, uint16_t count,
          bool) -> std::tuple<typename Op1::type, bool, bool> {
        if (count > sizeof(a) * 8) {
          return std::make_tuple(0, false, false);
        }
        if (count == sizeof(a) * 8) {
          return std::make_tuple(0, lsb(a), false);
        }
        decltype(a) result = a << count;
        auto cf = msb(a << (count - 1));
        // std::cout << (int)cf << " " << (int)result << " " <<
        // msb(result) << " "
        //           << (int)(result &
        //           (static_cast<decltype(result)>(1)
        //                              <<
        //                              (sizeof(decltype(result))
        //                              * 8 - 1)))
        //           << " "
        //           << (int)(static_cast<decltype(result)>(1)
        //                    << (sizeof(decltype(result)) * 8 -
        //                    1))
        //           << (msb(result) != cf) << std::endl;
        return std::make_tuple(result, cf, msb(result) != cf);
      });
}

}  // namespace emu::cpu::insts

#include "gen/Shl.cpp"
