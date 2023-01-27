#include <catch2/catch_test_macros.hpp>

#include <mos6502/cpu.hpp>

using namespace mos6502;

TEST_CASE("Set and Get Status Register Flags", "[cpu]")
{
  auto cpu = CPU{};

  SECTION("Carry Flag")
  {
    set_flag(cpu.SR, Flag::C, true);
    REQUIRE(get_flag(cpu.SR, Flag::C) == true);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::I) == true);
    REQUIRE(get_flag(cpu.SR, Flag::D) == false);
    REQUIRE(get_flag(cpu.SR, Flag::B) == false);
    REQUIRE(get_flag(cpu.SR, Flag::U) == true);
    REQUIRE(get_flag(cpu.SR, Flag::V) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);

    set_flag(cpu.SR, Flag::C, false);
    REQUIRE(get_flag(cpu.SR, Flag::C) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::I) == true);
    REQUIRE(get_flag(cpu.SR, Flag::D) == false);
    REQUIRE(get_flag(cpu.SR, Flag::B) == false);
    REQUIRE(get_flag(cpu.SR, Flag::U) == true);
    REQUIRE(get_flag(cpu.SR, Flag::V) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }

  set_flag(cpu.SR, Flag::C, false);
  REQUIRE(get_flag(cpu.SR, Flag::C) == false);


  set_flag(cpu.SR, Flag::Z, true);
  REQUIRE(get_flag(cpu.SR, Flag::Z) == true);

  set_flag(cpu.SR, Flag::Z, false);
  REQUIRE(get_flag(cpu.SR, Flag::Z) == false);


  set_flag(cpu.SR, Flag::N, true);
  REQUIRE(get_flag(cpu.SR, Flag::N) == true);

  set_flag(cpu.SR, Flag::N, true);
  REQUIRE(get_flag(cpu.SR, Flag::N) == true);

  set_flag(cpu.SR, Flag::N, false);
  REQUIRE(get_flag(cpu.SR, Flag::N) == false);

  set_flag(cpu.SR, Flag::N, false);
  REQUIRE(get_flag(cpu.SR, Flag::N) == false);

}
