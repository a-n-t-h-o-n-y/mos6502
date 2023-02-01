#include <catch2/catch_test_macros.hpp>

#include <mos6502/detail/bcd.hpp>
#include <mos6502/byte.hpp>

using namespace mos6502;
using namespace mos6502::detail;

TEST_CASE("BCD to BIN", "[bcd]")
{
  REQUIRE(BCD_to_BIN(0x00) == 0);
  REQUIRE(BCD_to_BIN(0x01) == 1);
  REQUIRE(BCD_to_BIN(0x10) == 10);
  REQUIRE(BCD_to_BIN(0x40) == 40);
  REQUIRE(BCD_to_BIN(0x45) == 45);
  REQUIRE(BCD_to_BIN(0x80) == 80);
  REQUIRE(BCD_to_BIN(0x85) == 85);
  REQUIRE(BCD_to_BIN(0x99) == 99);
}

TEST_CASE("BIN to BCD", "[bcd]")
{
  REQUIRE(BIN_to_BCD(0) == 0x00);
  REQUIRE(BIN_to_BCD(1) == 0x01);
  REQUIRE(BIN_to_BCD(10) == 0x10);
  REQUIRE(BIN_to_BCD(40) == 0x40);
  REQUIRE(BIN_to_BCD(45) == 0x45);
  REQUIRE(BIN_to_BCD(80) == 0x80);
  REQUIRE(BIN_to_BCD(85) == 0x85);
  REQUIRE(BIN_to_BCD(99) == 0x99);
}
