#include <catch2/catch_test_macros.hpp>

#include <nes/instructions.hpp>
#include <nes/cpu.hpp>
#include <nes/memory.hpp>

using namespace nes;

TEST_CASE("ADC", "[instructions]")
{
  auto cpu = CPU{};

  SECTION("Positive Numbers, No Signed Overflow, No Carry")
  {
    cpu.AC = 0x05;
    ADC(cpu, 0x05);
    REQUIRE(cpu.AC == 0x0A);
    REQUIRE(get_flag(cpu.SR, Flag::C) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::V) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }

  SECTION("Positive Numbers, No Signed Overflow, With Carry Already Set")
  {
    cpu.AC = 0x05;
    set_flag(cpu.SR, Flag::C, true);
    ADC(cpu, 0x05);
    REQUIRE(cpu.AC == 0x0B);
    REQUIRE(get_flag(cpu.SR, Flag::C) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::V) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }

  SECTION("Positive Numbers, No Signed Overflow, With Carry Produced")
  {
    cpu.AC = 0xF2;
    ADC(cpu, 0x0F);
    REQUIRE(cpu.AC == 0x01);
    REQUIRE(get_flag(cpu.SR, Flag::C) == true);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::V) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }

  SECTION("Zero Flag")
  {
    cpu.AC = 0x5A;
    ADC(cpu, 0xA6);
    REQUIRE(cpu.AC == 0x00);
    REQUIRE(get_flag(cpu.SR, Flag::C) == true);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
    REQUIRE(get_flag(cpu.SR, Flag::V) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }

  SECTION("Negative Result")
  {
    cpu.AC = 0x5A;
    ADC(cpu, 0xA1);
    REQUIRE(cpu.AC == 0xFB); // -5 in two's complement
    REQUIRE(get_flag(cpu.SR, Flag::C) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::V) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
  }

  SECTION("Positive Signed Overflow")
  {
    cpu.AC = 0x7F;
    ADC(cpu, 0x05);
    REQUIRE(cpu.AC == 0x84);
    REQUIRE(get_flag(cpu.SR, Flag::C) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::V) == true);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
  }

  SECTION("Negative Signed Overflow")
  {
    cpu.AC = 0x80;
    ADC(cpu, 0x80);
    REQUIRE(cpu.AC == 0x00);
    REQUIRE(get_flag(cpu.SR, Flag::C) == true);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
    REQUIRE(get_flag(cpu.SR, Flag::V) == true);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }
}

TEST_CASE("SBC", "[instructions]")
{
  // Borrow flag is inverse of Carry flag. Negation is used here to read easier

  auto cpu = CPU{};

  SECTION("Positive Numbers, No Signed Overflow, No Carry")
  {
    cpu.AC = 0x0F;
    set_flag(cpu.SR, Flag::C, !false);  // (Carry == true) is (Borrow == false)
    SBC(cpu, 0x05);
    REQUIRE(cpu.AC == 0x0A);
    REQUIRE(get_flag(cpu.SR, Flag::C) == !false);  // Again, Borrow == false
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::V) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }

  SECTION("Positive Numbers, No Signed Overflow, With Borrow Already Set")
  {
    cpu.AC = 0x0F;
    set_flag(cpu.SR, Flag::C, !true);
    SBC(cpu, 0x05);
    REQUIRE(cpu.AC == 0x09);
    REQUIRE(get_flag(cpu.SR, Flag::C) == !false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::V) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }

  SECTION("Positive Numbers, No Signed Overflow, With Borrow Produced")
  {
    cpu.AC = 0x0F;
    set_flag(cpu.SR, Flag::C, !false);
    SBC(cpu, 0x1F);
    REQUIRE(cpu.AC == 0xF0);
    REQUIRE(get_flag(cpu.SR, Flag::C) == !true);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::V) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
  }

  SECTION("Zero Flag")
  {
    cpu.AC = 0x5A;
    set_flag(cpu.SR, Flag::C, !false);
    SBC(cpu, 0x5A);
    REQUIRE(cpu.AC == 0x00);
    REQUIRE(get_flag(cpu.SR, Flag::C) == !false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
    REQUIRE(get_flag(cpu.SR, Flag::V) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }

  SECTION("Positive Signed Overflow")
  {
    cpu.AC = 0x7F;
    set_flag(cpu.SR, Flag::C, !false);
    SBC(cpu, 0xFB); // -5
    REQUIRE(cpu.AC == 0x84);
    REQUIRE(get_flag(cpu.SR, Flag::C) == !true);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::V) == true);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
  }

  SECTION("Negative Signed Overflow")
  {
    cpu.AC = 0x9C;  // -100
    set_flag(cpu.SR, Flag::C, !false);
    SBC(cpu, 0x64);  // 100
    REQUIRE(cpu.AC == 0x38); // +56
    REQUIRE(get_flag(cpu.SR, Flag::C) == !false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::V) == true);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }
}
