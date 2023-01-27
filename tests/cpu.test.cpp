#include <catch2/catch_test_macros.hpp>

#include <nes/cpu.hpp>

using namespace nes;

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

// TODO remove
// #include <nes/memory.hpp>
// #include <nes/timed_execute.hpp>

// TODO remove
TEST_CASE("Foo", "[wow]") {
  // auto cpu = CPU{};
  // auto mem = Memory{};
  // mem.write(0x0000, 0x42);

  // timed_execute<1'789'773>([]{return false;});

  // execute_next_opcode(cpu, mem);
  // execute_next_opcode(cpu, mem);
  // execute_next_opcode(cpu, mem);
  // execute_next_opcode(cpu, mem);
  // execute_next_opcode(cpu, mem);
  // execute_next_opcode(cpu, mem);
}
