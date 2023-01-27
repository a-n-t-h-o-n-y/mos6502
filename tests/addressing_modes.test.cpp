#include <catch2/catch_test_macros.hpp>

#include <mos6502/address.hpp>
#include <mos6502/addressing_modes.hpp>
#include <mos6502/byte_array.hpp>
#include <mos6502/cpu.hpp>

#include <algorithm>

using namespace mos6502;

TEST_CASE("Implied", "[addressing_modes]") { IMP(); }

TEST_CASE("Accumulator", "[addressing_modes]")
{
  auto cpu = CPU{};
  cpu.PC   = 0x0000;

  cpu.AC = 5;
  REQUIRE(ACC(cpu) == 5);
  REQUIRE(cpu.PC == 0x0000);

  cpu.AC = 255;
  REQUIRE(ACC(cpu) == 255);
  REQUIRE(cpu.PC == 0x0000);
}

TEST_CASE("Absolute", "[addressing_modes]")
{
  auto cpu = CPU{};
  auto mem = ByteArray{};

  cpu.PC = 0x0023;
  mem.write(cpu.PC, 0x34);
  mem.write(cpu.PC + 0x0001, 0x12);

  REQUIRE(ABS(cpu, mem) == 0x1234);
  REQUIRE(cpu.PC == 0x0025);
}

TEST_CASE("Absolute, X-indexed", "[addressing_modes]")
{
  auto mem = ByteArray{};
  auto cpu = CPU{};
  cpu.PC   = 0x0023;

  SECTION("No Page Crossing")
  {
    mem.write(cpu.PC, 0x34);
    mem.write(cpu.PC + 0x0001, 0x12);
    cpu.X = 0x05;
    REQUIRE(ABX(cpu, mem) == std::pair{false, Address{0x1239}});
    REQUIRE(cpu.PC == 0x0025);
  }

  SECTION("Page Crossing")
  {
    mem.write(cpu.PC, 0xFF);
    mem.write(cpu.PC + 0x0001, 0x01);
    cpu.X = 0x05;

    REQUIRE(ABX(cpu, mem) == std::pair{true, Address{0x0204}});
    REQUIRE(cpu.PC == 0x0025);
  }
}

TEST_CASE("Absolute, Y-indexed", "[addressing_modes]")
{
  auto mem = ByteArray{};
  auto cpu = CPU{};
  cpu.PC   = 0x0023;

  SECTION("No Page Crossing")
  {
    mem.write(cpu.PC, 0x34);
    mem.write(cpu.PC + 0x0001, 0x12);
    cpu.Y = 0x05;
    REQUIRE(ABY(cpu, mem) == std::pair{false, Address{0x1239}});
    REQUIRE(cpu.PC == 0x0025);
  }

  SECTION("Page Crossing")
  {
    mem.write(cpu.PC, 0xFF);
    mem.write(cpu.PC + 0x0001, 0x01);
    cpu.Y = 0x05;

    REQUIRE(ABY(cpu, mem) == std::pair{true, Address{0x0204}});
    REQUIRE(cpu.PC == 0x0025);
  }
}

TEST_CASE("Immediate", "[addressing_modes]")
{
  auto mem = ByteArray{};
  auto cpu = CPU{};
  cpu.PC   = 0x0023;
  mem.write(cpu.PC, 0x55);
  REQUIRE(IMM(cpu, mem) == 0x55);
  REQUIRE(cpu.PC == 0x0024);
}

TEST_CASE("Indirect", "[addressing_modes]")
{
  auto mem = ByteArray{};
  auto cpu = CPU{};
  SECTION("No Page Crossing")
  {
    cpu.PC = 0x0023;
    mem.write(cpu.PC, 0xAB);
    mem.write(cpu.PC + 0x0001, 0x05);

    mem.write(0x05AB, 0x34);
    mem.write(0x05AC, 0x12);

    REQUIRE(IND(cpu, mem) == 0x1234);
    REQUIRE(cpu.PC == 0x0025);
  }
  SECTION("Page Crossing With Bug")
  {
    cpu.PC = 0x0023;
    mem.write(cpu.PC, 0xFF);
    mem.write(cpu.PC + 0x0001, 0x05);

    mem.write(0x05FF, 0x34);
    mem.write(0x0500, 0x12);

    REQUIRE(IND(cpu, mem) == 0x1234);
    REQUIRE(cpu.PC == 0x0025);
  }
}

TEST_CASE("Indirect - X Indexed", "[addressing_modes]")
{
  auto mem = ByteArray{};
  auto cpu = CPU{};

  mem.write(0x0012, 0xCD);
  mem.write(0x0013, 0xAB);

  cpu.PC = 0x0823;
  mem.write(cpu.PC, 0x10);
  cpu.X = 0x02;

  REQUIRE(IZX(cpu, mem) == 0xABCD);
  REQUIRE(cpu.PC == 0x0824);
}

TEST_CASE("Indirect - Y Indexed", "[addressing_modes]")
{
  auto mem = ByteArray{};
  auto cpu = CPU{};

  SECTION("No Page Crossing")
  {
    mem.write(0x0012, 0x34);
    mem.write(0x0013, 0x12);

    cpu.PC = 0x0823;
    mem.write(cpu.PC, 0x12);
    cpu.Y = 0x02;

    REQUIRE(IZY(cpu, mem) == std::pair{false, Address{0x1236}});
    REQUIRE(cpu.PC == 0x0824);
  }
  SECTION("Page Crossing")
  {
    mem.write(0x0012, 0xFE);
    mem.write(0x0013, 0x12);

    cpu.PC = 0x0823;
    mem.write(cpu.PC, 0x12);
    cpu.Y = 0x02;

    REQUIRE(IZY(cpu, mem) == std::pair{true, Address{0x1300}});
    REQUIRE(cpu.PC == 0x0824);
  }
}

TEST_CASE("Relative", "[addressing_modes]")
{
  auto mem = ByteArray{};
  auto cpu = CPU{};

  SECTION("Positive Value")
  {
    cpu.PC = 0x0823;
    mem.write(cpu.PC, 0x05);

    REQUIRE(REL(cpu, mem) == (0x0824 + 0x05));  // PC is incremented by one
    REQUIRE(cpu.PC == 0x0824);
  }
  SECTION("Negative Value")
  {
    cpu.PC = 0x0823;
    mem.write(cpu.PC, 0xFD);  // -3

    REQUIRE(REL(cpu, mem) == (0x0824 - 0x03));  // PC is incremented by one
    REQUIRE(cpu.PC == 0x0824);
  }
  SECTION("Negative Value With MSB Set On PC")
  {
    cpu.PC = 0xF823;
    mem.write(cpu.PC, 0xFD);  // -3

    REQUIRE(REL(cpu, mem) == (0xF824 - 0x03));  // PC is incremented by one
    REQUIRE(cpu.PC == 0xF824);
  }
}

TEST_CASE("Zero Page", "[addressing_modes]")
{
  auto mem = ByteArray{};
  auto cpu = CPU{};

  cpu.PC = 0x0123;
  mem.write(cpu.PC, 0x5F);
  REQUIRE(ZP0(cpu, mem) == 0x005F);
  REQUIRE(cpu.PC == 0x0124);
}

TEST_CASE("Zero Page - X Indexed", "[addressing_modes]")
{
  auto mem = ByteArray{};
  auto cpu = CPU{};

  SECTION("No Carry")
  {
    cpu.X  = 0x10;
    cpu.PC = 0x0123;
    mem.write(cpu.PC, 0x5F);
    REQUIRE(ZPX(cpu, mem) == 0x006F);
    REQUIRE(cpu.PC == 0x0124);
  }
  SECTION("With Carry, Which Is Zeroed")
  {
    cpu.X  = 0xFF;
    cpu.PC = 0x0123;
    mem.write(cpu.PC, 0x5F);
    REQUIRE(ZPX(cpu, mem) == 0x005E);
    REQUIRE(cpu.PC == 0x0124);
  }
}

TEST_CASE("Zero Page - Y Indexed", "[addressing_modes]")
{
  auto mem = ByteArray{};
  auto cpu = CPU{};

  SECTION("No Carry")
  {
    cpu.Y  = 0x10;
    cpu.PC = 0x0123;
    mem.write(cpu.PC, 0x5F);
    REQUIRE(ZPY(cpu, mem) == 0x006F);
    REQUIRE(cpu.PC == 0x0124);
  }
  SECTION("With Carry, Which Is Zeroed")
  {
    cpu.Y  = 0xFF;
    cpu.PC = 0x0123;
    mem.write(cpu.PC, 0x5F);
    REQUIRE(ZPY(cpu, mem) == 0x005E);
    REQUIRE(cpu.PC == 0x0124);
  }
}
