// #include <catch2/catch_all.hpp>

// using namespace Catch;

// TEST_CASE("Quick check", "[main]") {
//   std::vector<double> values{1, 2., 3.};
//   auto [mean, moment] = std::make_tuple(2.0, 5);

//   REQUIRE(mean == 2.0);
//   REQUIRE(moment == Approx(4.666666));
// }

#include "EmuTest.h"
#include "gtest/gtest.h"

TEST_F(EmuTest, Cli1) {
  setInstructionData({0xfa});
  cpu.regs_.flags._if = true;
  cpu.step();
  ASSERT_EQ(cpu.regs_.flags._if, false);
}

TEST_F(EmuTest, Cli2) {
  setInstructionData({0xfa});
  cpu.regs_.flags._if = false;
  cpu.step();
  ASSERT_EQ(cpu.regs_.flags._if, false);
}
