#include <catch2/catch_test_macros.hpp>

#include <mos6502/cpu.hpp>
#include <mos6502/instructions.hpp>

using namespace mos6502;

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
    REQUIRE(cpu.AC == 0xFB);  // -5 in two's complement
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
    SBC(cpu, 0xFB);  // -5
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
    SBC(cpu, 0x64);          // 100
    REQUIRE(cpu.AC == 0x38);  // +56
    REQUIRE(get_flag(cpu.SR, Flag::C) == !false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::V) == true);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }
}

TEST_CASE("LDA", "[instructions]")
{
  auto cpu = CPU{};

  SECTION("Positive")
  {
    LDA(cpu, 0x05);
    REQUIRE(cpu.AC == 0x05);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }
  SECTION("Negative")
  {
    LDA(cpu, 0xFF);
    REQUIRE(cpu.AC == 0xFF);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
  }
  SECTION("Zero")
  {
    LDA(cpu, 0x00);
    REQUIRE(cpu.AC == 0x00);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }
}

TEST_CASE("LDX", "[instructions]")
{
  auto cpu = CPU{};

  SECTION("Positive")
  {
    LDX(cpu, 0x05);
    REQUIRE(cpu.X == 0x05);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }
  SECTION("Negative")
  {
    LDX(cpu, 0xFF);
    REQUIRE(cpu.X == 0xFF);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
  }
  SECTION("Zero")
  {
    LDX(cpu, 0x00);
    REQUIRE(cpu.X == 0x00);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }
}

TEST_CASE("LDY", "[instructions]")
{
  auto cpu = CPU{};

  SECTION("Positive")
  {
    LDY(cpu, 0x05);
    REQUIRE(cpu.Y == 0x05);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }
  SECTION("Negative")
  {
    LDY(cpu, 0xFF);
    REQUIRE(cpu.Y == 0xFF);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
  }
  SECTION("Zero")
  {
    LDY(cpu, 0x00);
    REQUIRE(cpu.Y == 0x00);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }
}

TEST_CASE("STA", "[instructions]")
{
  auto cpu = CPU{};
  auto mem = Memory{};

  cpu.AC = 0x77;
  STA(cpu, mem, 0x0123);
  REQUIRE(mem.read(0x0123) == 0x77);
}

TEST_CASE("STX", "[instructions]")
{
  auto cpu = CPU{};
  auto mem = Memory{};

  cpu.X = 0x77;
  STX(cpu, mem, 0x0123);
  REQUIRE(mem.read(0x0123) == 0x77);
}

TEST_CASE("STY", "[instructions]")
{
  auto cpu = CPU{};
  auto mem = Memory{};

  cpu.Y = 0x77;
  STY(cpu, mem, 0x0123);
  REQUIRE(mem.read(0x0123) == 0x77);
}

TEST_CASE("TAX", "[instructions]")
{
  auto cpu = CPU{};

  SECTION("Positive")
  {
    cpu.AC = 0x05;
    TAX(cpu);
    REQUIRE(cpu.X == 0x05);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }
  SECTION("Negative")
  {
    cpu.AC = 0xFF;
    TAX(cpu);
    REQUIRE(cpu.X == 0xFF);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
  }
  SECTION("Zero")
  {
    cpu.AC = 0x00;
    TAX(cpu);
    REQUIRE(cpu.X == 0x00);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }
}

TEST_CASE("TAY", "[instructions]")
{
  auto cpu = CPU{};

  SECTION("Positive")
  {
    cpu.AC = 0x05;
    TAY(cpu);
    REQUIRE(cpu.Y == 0x05);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }
  SECTION("Negative")
  {
    cpu.AC = 0xFF;
    TAY(cpu);
    REQUIRE(cpu.Y == 0xFF);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
  }
  SECTION("Zero")
  {
    cpu.AC = 0x00;
    TAY(cpu);
    REQUIRE(cpu.Y == 0x00);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }
}

TEST_CASE("TSX", "[instructions]")
{
  auto cpu = CPU{};

  SECTION("Positive")
  {
    cpu.SP = 0x05;
    TSX(cpu);
    REQUIRE(cpu.X == 0x05);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }
  SECTION("Negative")
  {
    cpu.SP = 0xFF;
    TSX(cpu);
    REQUIRE(cpu.X == 0xFF);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
  }
  SECTION("Zero")
  {
    cpu.SP = 0x00;
    TSX(cpu);
    REQUIRE(cpu.X == 0x00);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }
}

TEST_CASE("TXA", "[instructions]")
{
  auto cpu = CPU{};

  SECTION("Positive")
  {
    cpu.X = 0x05;
    TXA(cpu);
    REQUIRE(cpu.AC == 0x05);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }
  SECTION("Negative")
  {
    cpu.X = 0xFF;
    TXA(cpu);
    REQUIRE(cpu.AC == 0xFF);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
  }
  SECTION("Zero")
  {
    cpu.X = 0x00;
    TXA(cpu);
    REQUIRE(cpu.AC == 0x00);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }
}

TEST_CASE("TXS", "[instructions]")
{
  auto cpu = CPU{};

  SECTION("Positive")
  {
    cpu.X = 0x05;
    TXS(cpu);
    REQUIRE(cpu.SP == 0x05);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }
  SECTION("Negative")
  {
    cpu.X = 0xFF;
    TXS(cpu);
    REQUIRE(cpu.SP == 0xFF);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
  }
  SECTION("Zero")
  {
    cpu.X = 0x00;
    TXS(cpu);
    REQUIRE(cpu.SP == 0x00);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }
}

TEST_CASE("TYA", "[instructions]")
{
  auto cpu = CPU{};

  SECTION("Positive")
  {
    cpu.Y = 0x05;
    TYA(cpu);
    REQUIRE(cpu.AC == 0x05);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }
  SECTION("Negative")
  {
    cpu.Y = 0xFF;
    TYA(cpu);
    REQUIRE(cpu.AC == 0xFF);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
  }
  SECTION("Zero")
  {
    cpu.Y = 0x00;
    TYA(cpu);
    REQUIRE(cpu.AC == 0x00);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  }
}

TEST_CASE("PHA", "[instructions]")
{
  constexpr Address STACK_TOP = 0x0100 + 0x00FF;

  auto cpu = CPU{};
  auto mem = Memory{};

  cpu.AC = 0x5A;
  PHA(cpu, mem);

  REQUIRE(mem.read(STACK_TOP) == 0x5A);
  REQUIRE(cpu.SP == (0xFF - 1));
}

TEST_CASE("PHP", "[instructions]")
{
  constexpr Address STACK_TOP = 0x0100 + 0x00FF;

  auto cpu = CPU{};
  auto mem = Memory{};

  set_flag(cpu.SR, Flag::C, true);
  set_flag(cpu.SR, Flag::I, true);
  set_flag(cpu.SR, Flag::V, true);

  PHP(cpu, mem);

  Byte const pushed = mem.read(STACK_TOP);

  REQUIRE(get_flag(pushed, Flag::C) == true);
  REQUIRE(get_flag(pushed, Flag::I) == true);
  REQUIRE(get_flag(pushed, Flag::V) == true);
  REQUIRE(get_flag(pushed, Flag::B) == true);
  REQUIRE(get_flag(pushed, Flag::U) == true);
  REQUIRE(get_flag(pushed, Flag::D) == false);
  REQUIRE(get_flag(pushed, Flag::N) == false);
  REQUIRE(get_flag(pushed, Flag::Z) == false);


  REQUIRE(cpu.SP == (0xFF - 1));
  REQUIRE(get_flag(cpu.SR, Flag::C) == true);
  REQUIRE(get_flag(cpu.SR, Flag::I) == true);
  REQUIRE(get_flag(cpu.SR, Flag::V) == true);
  REQUIRE(get_flag(cpu.SR, Flag::B) == false);
  REQUIRE(get_flag(cpu.SR, Flag::U) == true);
  REQUIRE(get_flag(cpu.SR, Flag::D) == false);
  REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
}

TEST_CASE("PLA", "[instructions]")
{
  auto cpu = CPU{};
  auto mem = Memory{};

  SECTION("Positive") {
    cpu.AC = 0x5A;
    PHA(cpu, mem);

    // Reset accumulator, then pull old accumulator value from stack.
    cpu.AC = 0x00;
    PLA(cpu, mem);

    REQUIRE(cpu.AC == 0x5A);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Negative") {
    cpu.AC = 0x8A;
    PHA(cpu, mem);

    // Reset accumulator, then pull old accumulator value from stack.
    cpu.AC = 0x00;
    PLA(cpu, mem);

    REQUIRE(cpu.AC == 0x8A);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Zero") {
    cpu.AC = 0x00;
    PHA(cpu, mem);

    // Reset accumulator, then pull old accumulator value from stack.
    cpu.AC = 0x00;
    PLA(cpu, mem);

    REQUIRE(cpu.AC == 0x00);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
  }
}

TEST_CASE("PLP", "[instructions]")
{
  constexpr Address STACK_TOP = 0x0100 + 0x00FF;

  auto cpu = CPU{};
  auto mem = Memory{};

  set_flag(cpu.SR, Flag::C, true);
  set_flag(cpu.SR, Flag::I, true);
  set_flag(cpu.SR, Flag::V, true);

  PHP(cpu, mem);

  Byte const pushed = mem.read(STACK_TOP);

  REQUIRE(get_flag(pushed, Flag::C) == true);
  REQUIRE(get_flag(pushed, Flag::I) == true);
  REQUIRE(get_flag(pushed, Flag::V) == true);
  REQUIRE(get_flag(pushed, Flag::B) == true);
  REQUIRE(get_flag(pushed, Flag::U) == true);
  REQUIRE(get_flag(pushed, Flag::D) == false);
  REQUIRE(get_flag(pushed, Flag::N) == false);
  REQUIRE(get_flag(pushed, Flag::Z) == false);


  REQUIRE(cpu.SP == (0xFF - 1));
  REQUIRE(get_flag(cpu.SR, Flag::C) == true);
  REQUIRE(get_flag(cpu.SR, Flag::I) == true);
  REQUIRE(get_flag(cpu.SR, Flag::V) == true);
  REQUIRE(get_flag(cpu.SR, Flag::B) == false);
  REQUIRE(get_flag(cpu.SR, Flag::U) == true);
  REQUIRE(get_flag(cpu.SR, Flag::D) == false);
  REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  REQUIRE(get_flag(cpu.SR, Flag::Z) == false);

  cpu.SR = 0xFF;

  PLP(cpu, mem);
  REQUIRE(cpu.SP == 0xFF);
  REQUIRE(get_flag(cpu.SR, Flag::C) == true);
  REQUIRE(get_flag(cpu.SR, Flag::I) == true);
  REQUIRE(get_flag(cpu.SR, Flag::V) == true);
  REQUIRE(get_flag(cpu.SR, Flag::B) == false);
  REQUIRE(get_flag(cpu.SR, Flag::U) == false);
  REQUIRE(get_flag(cpu.SR, Flag::D) == false);
  REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
}

TEST_CASE("DEC", "[instructions]")
{
  auto cpu = CPU{};
  auto mem = Memory{};
  auto adr = Address{0x0052};

  SECTION("Positive")
  {
    mem.write(adr, 0x12);
    DEC(cpu, mem, adr);
    REQUIRE(mem.read(adr) == 0x11);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Negative")
  {
    mem.write(adr, 0x00);
    DEC(cpu, mem, adr);
    REQUIRE(mem.read(adr) == 0xFF);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Zero")
  {
    mem.write(adr, 0x01);
    DEC(cpu, mem, adr);
    REQUIRE(mem.read(adr) == 0x00);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
  }
}

TEST_CASE("DEX", "[instructions]")
{
  auto cpu = CPU{};

  SECTION("Positive")
  {
    cpu.X = 0x12;
    DEX(cpu);
    REQUIRE(cpu.X == 0x11);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Negative")
  {
    cpu.X = 0x00;
    DEX(cpu);
    REQUIRE(cpu.X == 0xFF);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Zero")
  {
    cpu.X = 0x01;
    DEX(cpu);
    REQUIRE(cpu.X == 0x00);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
  }
}

TEST_CASE("DEY", "[instructions]")
{
  auto cpu = CPU{};

  SECTION("Positive")
  {
    cpu.Y = 0x12;
    DEY(cpu);
    REQUIRE(cpu.Y == 0x11);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Negative")
  {
    cpu.Y = 0x00;
    DEY(cpu);
    REQUIRE(cpu.Y == 0xFF);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Zero")
  {
    cpu.Y = 0x01;
    DEY(cpu);
    REQUIRE(cpu.Y == 0x00);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
  }
}

TEST_CASE("INC", "[instructions]")
{
  auto cpu = CPU{};
  auto mem = Memory{};
  auto adr = Address{0x0052};

  SECTION("Positive")
  {
    mem.write(adr, 0x12);
    INC(cpu, mem, adr);
    REQUIRE(mem.read(adr) == 0x13);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Negative")
  {
    mem.write(adr, 0xFE);
    INC(cpu, mem, adr);
    REQUIRE(mem.read(adr) == 0xFF);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Zero")
  {
    mem.write(adr, 0xFF);
    INC(cpu, mem, adr);
    REQUIRE(mem.read(adr) == 0x00);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
  }
}

TEST_CASE("INX", "[instructions]")
{
  auto cpu = CPU{};

  SECTION("Positive")
  {
    cpu.X = 0x12;
    INX(cpu);
    REQUIRE(cpu.X == 0x13);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Negative")
  {
    cpu.X = 0xFE;
    INX(cpu);
    REQUIRE(cpu.X == 0xFF);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Zero")
  {
    cpu.X = 0xFF;
    INX(cpu);
    REQUIRE(cpu.X == 0x00);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
  }
}

TEST_CASE("INY", "[instructions]")
{
  auto cpu = CPU{};

  SECTION("Positive")
  {
    cpu.Y = 0x12;
    INY(cpu);
    REQUIRE(cpu.Y == 0x13);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Negative")
  {
    cpu.Y = 0xFE;
    INY(cpu);
    REQUIRE(cpu.Y == 0xFF);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Zero")
  {
    cpu.Y = 0xFF;
    INY(cpu);
    REQUIRE(cpu.Y == 0x00);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
  }
}

TEST_CASE("AND", "[instructions]")
{
  auto cpu = CPU{};
  SECTION("Positive")
  {
    cpu.AC = 0b01010101;
    AND(cpu, 0b00100101);
    REQUIRE(cpu.AC == 0b00000101);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Negative")
  {
    cpu.AC = 0b10001011;
    AND(cpu, 0b11101111);
    REQUIRE(cpu.AC == 0b10001011);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Zero")
  {
    cpu.AC = 0b11110000;
    AND(cpu, 0b00001111);
    REQUIRE(cpu.AC == 0b00000000);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
  }
}

TEST_CASE("EOR", "[instructions]")
{
  auto cpu = CPU{};
  SECTION("Positive")
  {
    cpu.AC = 0b01010101;
    EOR(cpu, 0b00100101);
    REQUIRE(cpu.AC == 0b01110000);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Negative")
  {
    cpu.AC = 0b10001011;
    EOR(cpu, 0b01101111);
    REQUIRE(cpu.AC == 0b11100100);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Zero")
  {
    cpu.AC = 0b11110000;
    EOR(cpu, 0b11110000);
    REQUIRE(cpu.AC == 0b00000000);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
  }
}

TEST_CASE("ORA", "[instructions]")
{
  auto cpu = CPU{};
  SECTION("Positive")
  {
    cpu.AC = 0b01010101;
    ORA(cpu, 0b00100101);
    REQUIRE(cpu.AC == 0b01110101);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Negative")
  {
    cpu.AC = 0b10001011;
    ORA(cpu, 0b01101111);
    REQUIRE(cpu.AC == 0b11101111);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Zero")
  {
    cpu.AC = 0b00000000;
    ORA(cpu, 0b00000000);
    REQUIRE(cpu.AC == 0b00000000);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
  }
}

TEST_CASE("ASL", "[instructions]")
{
  auto cpu = CPU{};
  SECTION("Accumulator")
  {
    SECTION("Positive") {
      cpu.AC = 0b00010001;
      ASL_ACC(cpu, cpu.AC);
      REQUIRE(cpu.AC == 0b00100010);
      REQUIRE(get_flag(cpu.SR, Flag::C) == false);
      REQUIRE(get_flag(cpu.SR, Flag::N) == false);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    }
    SECTION("Negative") {
      ASL_ACC(cpu, 0b01010001);
      REQUIRE(cpu.AC == 0b10100010);
      REQUIRE(get_flag(cpu.SR, Flag::C) == false);
      REQUIRE(get_flag(cpu.SR, Flag::N) == true);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    }
    SECTION("Zero") {
      ASL_ACC(cpu, 0b00000000);
      REQUIRE(cpu.AC == 0b00000000);
      REQUIRE(get_flag(cpu.SR, Flag::C) == false);
      REQUIRE(get_flag(cpu.SR, Flag::N) == false);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
    }
    SECTION("Carry") {
      cpu.AC = 0b10000010;
      ASL_ACC(cpu, cpu.AC);
      REQUIRE(cpu.AC == 0b00000100);
      REQUIRE(get_flag(cpu.SR, Flag::C) == true);
      REQUIRE(get_flag(cpu.SR, Flag::N) == false);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    }
  }
  SECTION("Memory")
  {
    auto mem       = Memory{};
    auto const adr = Address{0x0123};

    SECTION("Positive") {
      mem.write(adr, 0b00010001);
      ASL_MEM(cpu, mem, adr);
      REQUIRE(mem.read(adr) == 0b00100010);
      REQUIRE(get_flag(cpu.SR, Flag::C) == false);
      REQUIRE(get_flag(cpu.SR, Flag::N) == false);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    }
    SECTION("Negative") {
      mem.write(adr, 0b01010001);
      ASL_MEM(cpu, mem, adr);
      REQUIRE(mem.read(adr) == 0b10100010);
      REQUIRE(get_flag(cpu.SR, Flag::C) == false);
      REQUIRE(get_flag(cpu.SR, Flag::N) == true);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    }
    SECTION("Zero") {
      mem.write(adr, 0b00000000);
      ASL_MEM(cpu, mem, adr);
      REQUIRE(mem.read(adr) == 0b00000000);
      REQUIRE(get_flag(cpu.SR, Flag::C) == false);
      REQUIRE(get_flag(cpu.SR, Flag::N) == false);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
    }
    SECTION("Carry") {
      mem.write(adr, 0b10000010);
      ASL_MEM(cpu, mem, adr);
      REQUIRE(mem.read(adr) == 0b00000100);
      REQUIRE(get_flag(cpu.SR, Flag::C) == true);
      REQUIRE(get_flag(cpu.SR, Flag::N) == false);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    }
  }
}

TEST_CASE("LSR", "[instructions]")
{
  auto cpu = CPU{};
  SECTION("Accumulator")
  {
    SECTION("Positive") {
      LSR_ACC(cpu, 0b00010010);
      REQUIRE(cpu.AC == 0b00001001);
      REQUIRE(get_flag(cpu.SR, Flag::C) == false);
      REQUIRE(get_flag(cpu.SR, Flag::N) == false);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    }
    SECTION("Zero") {
      cpu.AC = 0b00000000;
      LSR_ACC(cpu, cpu.AC);
      REQUIRE(cpu.AC == 0b00000000);
      REQUIRE(get_flag(cpu.SR, Flag::C) == false);
      REQUIRE(get_flag(cpu.SR, Flag::N) == false);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
    }
    SECTION("Carry") {
      LSR_ACC(cpu, 0b10000001);
      REQUIRE(cpu.AC == 0b01000000);
      REQUIRE(get_flag(cpu.SR, Flag::C) == true);
      REQUIRE(get_flag(cpu.SR, Flag::N) == false);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    }
  }
  SECTION("Memory")
  {
    auto mem       = Memory{};
    auto const adr = Address{0x0123};

    SECTION("Positive") {
      mem.write(adr, 0b00010010);
      LSR_MEM(cpu, mem, adr);
      REQUIRE(mem.read(adr) == 0b00001001);
      REQUIRE(get_flag(cpu.SR, Flag::C) == false);
      REQUIRE(get_flag(cpu.SR, Flag::N) == false);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    }
    SECTION("Zero") {
      mem.write(adr, 0b00000000);
      LSR_MEM(cpu, mem, adr);
      REQUIRE(mem.read(adr) == 0b00000000);
      REQUIRE(get_flag(cpu.SR, Flag::C) == false);
      REQUIRE(get_flag(cpu.SR, Flag::N) == false);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
    }
    SECTION("Carry") {
      mem.write(adr, 0b10000001);
      LSR_MEM(cpu, mem, adr);
      REQUIRE(mem.read(adr) == 0b01000000);
      REQUIRE(get_flag(cpu.SR, Flag::C) == true);
      REQUIRE(get_flag(cpu.SR, Flag::N) == false);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    }
  }
}

TEST_CASE("ROL", "[instructions]")
{
  auto cpu = CPU{};
  SECTION("Accumulator")
  {
    SECTION("No Carry") {
      ROL_ACC(cpu, 0b00010010);
      REQUIRE(cpu.AC == 0b00100100);
      REQUIRE(get_flag(cpu.SR, Flag::C) == false);
      REQUIRE(get_flag(cpu.SR, Flag::N) == false);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    }
    SECTION("Initial Carry Set") {
      set_flag(cpu.SR, Flag::C, true);
      ROL_ACC(cpu, 0b00100000);
      REQUIRE(cpu.AC == 0b01000001);
      REQUIRE(get_flag(cpu.SR, Flag::C) == false);
      REQUIRE(get_flag(cpu.SR, Flag::N) == false);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    }
    SECTION("Negative") {
      ROL_ACC(cpu, 0b01000001);
      REQUIRE(cpu.AC == 0b10000010);
      REQUIRE(get_flag(cpu.SR, Flag::C) == false);
      REQUIRE(get_flag(cpu.SR, Flag::N) == true);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    }
    SECTION("Carry and Zero Are Set") {
      cpu.AC = 0b10000000;
      ROL_ACC(cpu, cpu.AC);
      REQUIRE(cpu.AC == 0b00000000);
      REQUIRE(get_flag(cpu.SR, Flag::C) == true);
      REQUIRE(get_flag(cpu.SR, Flag::N) == false);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
    }
  }
  SECTION("Memory")
  {
    auto mem       = Memory{};
    auto const adr = Address{0x0123};

    SECTION("No Carry") {
      mem.write(adr, 0b00010010);
      ROL_MEM(cpu, mem, adr);
      REQUIRE(mem.read(adr) == 0b00100100);
      REQUIRE(get_flag(cpu.SR, Flag::C) == false);
      REQUIRE(get_flag(cpu.SR, Flag::N) == false);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    }
    SECTION("Initial Carry Set") {
      set_flag(cpu.SR, Flag::C, true);
      mem.write(adr, 0b00100000);
      ROL_MEM(cpu, mem, adr);
      REQUIRE(mem.read(adr) == 0b01000001);
      REQUIRE(get_flag(cpu.SR, Flag::C) == false);
      REQUIRE(get_flag(cpu.SR, Flag::N) == false);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    }
    SECTION("Negative") {
      mem.write(adr, 0b01000001);
      ROL_MEM(cpu, mem, adr);
      REQUIRE(mem.read(adr) == 0b10000010);
      REQUIRE(get_flag(cpu.SR, Flag::C) == false);
      REQUIRE(get_flag(cpu.SR, Flag::N) == true);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    }
    SECTION("Carry and Zero Are Set") {
      mem.write(adr, 0b10000000);
      ROL_MEM(cpu, mem, adr);
      REQUIRE(mem.read(adr) == 0b00000000);
      REQUIRE(get_flag(cpu.SR, Flag::C) == true);
      REQUIRE(get_flag(cpu.SR, Flag::N) == false);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
    }
  }
}

TEST_CASE("ROR", "[instructions]")
{
  auto cpu = CPU{};
  SECTION("Accumulator")
  {
    SECTION("No Carry") {
      cpu.AC = 0b00010010;
      ROR_ACC(cpu, cpu.AC);
      REQUIRE(cpu.AC == 0b00001001);
      REQUIRE(get_flag(cpu.SR, Flag::C) == false);
      REQUIRE(get_flag(cpu.SR, Flag::N) == false);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    }
    SECTION("Initial Carry Set") {
      set_flag(cpu.SR, Flag::C, true);
      ROR_ACC(cpu, 0b00100000);
      REQUIRE(cpu.AC == 0b10010000);
      REQUIRE(get_flag(cpu.SR, Flag::C) == false);
      REQUIRE(get_flag(cpu.SR, Flag::N) == true);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    }
    SECTION("Carry and Zero Are Set") {
      ROR_ACC(cpu, 0b00000001);
      REQUIRE(cpu.AC == 0b00000000);
      REQUIRE(get_flag(cpu.SR, Flag::C) == true);
      REQUIRE(get_flag(cpu.SR, Flag::N) == false);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
    }
  }
  SECTION("Memory")
  {
    auto mem       = Memory{};
    auto const adr = Address{0x0123};

    SECTION("No Carry")
    {
      mem.write(adr, 0b00010010);
      ROR_MEM(cpu, mem, adr);
      REQUIRE(mem.read(adr) == 0b00001001);
      REQUIRE(get_flag(cpu.SR, Flag::C) == false);
      REQUIRE(get_flag(cpu.SR, Flag::N) == false);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    }
    SECTION("Initial Carry Set")
    {
      set_flag(cpu.SR, Flag::C, true);
      mem.write(adr, 0b00100000);
      ROR_MEM(cpu, mem, adr);
      REQUIRE(mem.read(adr) == 0b10010000);
      REQUIRE(get_flag(cpu.SR, Flag::C) == false);
      REQUIRE(get_flag(cpu.SR, Flag::N) == true);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
    }
    SECTION("Carry and Zero Are Set")
    {
      mem.write(adr, 0b00000001);
      ROR_MEM(cpu, mem, adr);
      REQUIRE(mem.read(adr) == 0b00000000);
      REQUIRE(get_flag(cpu.SR, Flag::C) == true);
      REQUIRE(get_flag(cpu.SR, Flag::N) == false);
      REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
    }
  }
}

TEST_CASE("Flag Instructions", "[instructions]")
{
  auto cpu = CPU{};

  SECTION("CLC")
  {
    set_flag(cpu.SR, Flag::C, true);
    CLC(cpu);
    REQUIRE(get_flag(cpu.SR, Flag::C) == false);
  }
  SECTION("CLD")
  {
    set_flag(cpu.SR, Flag::D, true);
    CLD(cpu);
    REQUIRE(get_flag(cpu.SR, Flag::D) == false);
  }
  SECTION("CLI")
  {
    set_flag(cpu.SR, Flag::I, true);
    CLI(cpu);
    REQUIRE(get_flag(cpu.SR, Flag::I) == false);
  }
  SECTION("CLV")
  {
    set_flag(cpu.SR, Flag::V, true);
    CLV(cpu);
    REQUIRE(get_flag(cpu.SR, Flag::V) == false);
  }
  SECTION("SEC")
  {
    set_flag(cpu.SR, Flag::C, false);
    SEC(cpu);
    REQUIRE(get_flag(cpu.SR, Flag::C) == true);
  }
  SECTION("SED")
  {
    set_flag(cpu.SR, Flag::D, false);
    SED(cpu);
    REQUIRE(get_flag(cpu.SR, Flag::D) == true);
  }
  SECTION("SEI")
  {
    set_flag(cpu.SR, Flag::I, false);
    SEI(cpu);
    REQUIRE(get_flag(cpu.SR, Flag::I) == true);
  }
}

TEST_CASE("CMP", "[instructions]")
{
  auto cpu = CPU{};

  // Carry set if reg >= memory

  SECTION("Positive, Sets Carry")
  {
    cpu.AC = 0x7F;
    CMP(cpu, 0x10);
    REQUIRE(get_flag(cpu.SR, Flag::C) == true);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Zero, Sets Carry")
  {
    cpu.AC = 0x35;
    CMP(cpu, 0x35);
    REQUIRE(get_flag(cpu.SR, Flag::C) == true);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
  }
  SECTION("Negative")
  {
    cpu.AC = 0x35;
    CMP(cpu, 0x7F);
    REQUIRE(get_flag(cpu.SR, Flag::C) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
}

TEST_CASE("CPX", "[instructions]")
{
  auto cpu = CPU{};

  // Carry set if reg >= memory

  SECTION("Positive, Sets Carry")
  {
    cpu.X = 0x7F;
    CPX(cpu, 0x10);
    REQUIRE(get_flag(cpu.SR, Flag::C) == true);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Zero, Sets Carry")
  {
    cpu.X = 0x35;
    CPX(cpu, 0x35);
    REQUIRE(get_flag(cpu.SR, Flag::C) == true);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
  }
  SECTION("Negative")
  {
    cpu.X = 0x35;
    CPX(cpu, 0x7F);
    REQUIRE(get_flag(cpu.SR, Flag::C) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
}

TEST_CASE("CPY", "[instructions]")
{
  auto cpu = CPU{};

  // Carry set if reg >= memory

  SECTION("Positive, Sets Carry")
  {
    cpu.Y = 0x7F;
    CPY(cpu, 0x10);
    REQUIRE(get_flag(cpu.SR, Flag::C) == true);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
  SECTION("Zero, Sets Carry")
  {
    cpu.Y = 0x35;
    CPY(cpu, 0x35);
    REQUIRE(get_flag(cpu.SR, Flag::C) == true);
    REQUIRE(get_flag(cpu.SR, Flag::N) == false);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == true);
  }
  SECTION("Negative")
  {
    cpu.Y = 0x35;
    CPY(cpu, 0x7F);
    REQUIRE(get_flag(cpu.SR, Flag::C) == false);
    REQUIRE(get_flag(cpu.SR, Flag::N) == true);
    REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  }
}

TEST_CASE("BCC", "[instructions]")
{
  Address const TARGET = 0x1000;

  auto cpu = CPU{};
  cpu.PC   = 0x0100;

  SECTION("BCC should branch when carry flag is clear")
  {
    set_flag(cpu.SR, Flag::C, false);
    int const cycles = BCC(cpu, TARGET);
    REQUIRE(cpu.PC == TARGET);
    REQUIRE(cycles == 2);
  }

  SECTION("BCC should not branch when carry flag is set")
  {
    set_flag(cpu.SR, Flag::C, true);
    int const cycles = BCC(cpu, TARGET);
    REQUIRE(cpu.PC == 0x0100);
    REQUIRE(cycles == 0);
  }

  SECTION("BCC should return one extra cycle on branch to same page")
  {
    cpu.PC = 0x1070;

    set_flag(cpu.SR, Flag::C, false);
    int const cycles = BCC(cpu, TARGET);
    REQUIRE(cpu.PC == TARGET);
    REQUIRE(cycles == 1);
  }
}

TEST_CASE("BCS", "[instructions]")
{
  Address const TARGET = 0x1000;

  auto cpu = CPU{};
  cpu.PC   = 0x0100;

  SECTION("BCS should branch when carry flag is set")
  {
    set_flag(cpu.SR, Flag::C, true);
    int const cycles = BCS(cpu, TARGET);
    REQUIRE(cpu.PC == TARGET);
    REQUIRE(cycles == 2);
  }

  SECTION("BCS should not branch when carry flag is clear")
  {
    set_flag(cpu.SR, Flag::C, false);
    int const cycles = BCS(cpu, TARGET);
    REQUIRE(cpu.PC == 0x0100);
    REQUIRE(cycles == 0);
  }

  SECTION("BCS should return one extra cycle on branch to same page")
  {
    cpu.PC = 0x1070;

    set_flag(cpu.SR, Flag::C, true);
    int const cycles = BCS(cpu, TARGET);
    REQUIRE(cpu.PC == TARGET);
    REQUIRE(cycles == 1);
  }
}

TEST_CASE("BEQ", "[instructions]")
{
  Address const TARGET = 0x1000;

  auto cpu = CPU{};
  cpu.PC   = 0x0100;

  SECTION("BEQ should branch when zero flag is set")
  {
    set_flag(cpu.SR, Flag::Z, true);
    int const cycles = BEQ(cpu, TARGET);
    REQUIRE(cpu.PC == TARGET);
    REQUIRE(cycles == 2);
  }

  SECTION("BEQ should not branch when zero flag is clear")
  {
    set_flag(cpu.SR, Flag::Z, false);
    int const cycles = BEQ(cpu, TARGET);
    REQUIRE(cpu.PC == 0x0100);
    REQUIRE(cycles == 0);
  }

  SECTION("BEQ should return one extra cycle on branch to same page")
  {
    cpu.PC = 0x1070;

    set_flag(cpu.SR, Flag::Z, true);
    int const cycles = BEQ(cpu, TARGET);
    REQUIRE(cpu.PC == TARGET);
    REQUIRE(cycles == 1);
  }
}

TEST_CASE("BMI", "[instructions]")
{
  Address const TARGET = 0x1000;

  auto cpu = CPU{};
  cpu.PC   = 0x0100;

  SECTION("BMI should branch when negative flag is set")
  {
    set_flag(cpu.SR, Flag::N, true);
    int const cycles = BMI(cpu, TARGET);
    REQUIRE(cpu.PC == TARGET);
    REQUIRE(cycles == 2);
  }

  SECTION("BMI should not branch when negative flag is clear")
  {
    set_flag(cpu.SR, Flag::N, false);
    int const cycles = BMI(cpu, TARGET);
    REQUIRE(cpu.PC == 0x0100);
    REQUIRE(cycles == 0);
  }

  SECTION("BMI should return one extra cycle on branch to same page")
  {
    cpu.PC = 0x1070;

    set_flag(cpu.SR, Flag::N, true);
    int const cycles = BMI(cpu, TARGET);
    REQUIRE(cpu.PC == TARGET);
    REQUIRE(cycles == 1);
  }
}

TEST_CASE("BNE", "[instructions]")
{
  Address const TARGET = 0x1000;

  auto cpu = CPU{};
  cpu.PC   = 0x0100;

  SECTION("BNE should branch when zero flag is clear")
  {
    set_flag(cpu.SR, Flag::Z, false);
    int const cycles = BNE(cpu, TARGET);
    REQUIRE(cpu.PC == TARGET);
    REQUIRE(cycles == 2);
  }

  SECTION("BNE should not branch when zero flag is set")
  {
    set_flag(cpu.SR, Flag::Z, true);
    int const cycles = BNE(cpu, TARGET);
    REQUIRE(cpu.PC == 0x0100);
    REQUIRE(cycles == 0);
  }

  SECTION("BNE should return one extra cycle on branch to same page")
  {
    cpu.PC = 0x1070;

    set_flag(cpu.SR, Flag::Z, false);
    int const cycles = BNE(cpu, TARGET);
    REQUIRE(cpu.PC == TARGET);
    REQUIRE(cycles == 1);
  }
}

TEST_CASE("BPL", "[instructions]")
{
  Address const TARGET = 0x1000;

  auto cpu = CPU{};
  cpu.PC   = 0x0100;

  SECTION("BPL should branch when negative flag is clear")
  {
    set_flag(cpu.SR, Flag::N, false);
    int const cycles = BPL(cpu, TARGET);
    REQUIRE(cpu.PC == TARGET);
    REQUIRE(cycles == 2);
  }

  SECTION("BPL should not branch when negative flag is set")
  {
    set_flag(cpu.SR, Flag::N, true);
    int const cycles = BPL(cpu, TARGET);
    REQUIRE(cpu.PC == 0x0100);
    REQUIRE(cycles == 0);
  }

  SECTION("BPL should return one extra cycle on branch to same page")
  {
    cpu.PC = 0x1070;

    set_flag(cpu.SR, Flag::N, false);
    int const cycles = BPL(cpu, TARGET);
    REQUIRE(cpu.PC == TARGET);
    REQUIRE(cycles == 1);
  }
}

TEST_CASE("BVC", "[instructions]")
{
  Address const TARGET = 0x1000;

  auto cpu = CPU{};
  cpu.PC   = 0x0100;

  SECTION("BVC should branch when overflow flag is clear")
  {
    set_flag(cpu.SR, Flag::V, false);
    int const cycles = BVC(cpu, TARGET);
    REQUIRE(cpu.PC == TARGET);
    REQUIRE(cycles == 2);
  }

  SECTION("BVC should not branch when overflow flag is set")
  {
    set_flag(cpu.SR, Flag::V, true);
    int const cycles = BVC(cpu, TARGET);
    REQUIRE(cpu.PC == 0x0100);
    REQUIRE(cycles == 0);
  }

  SECTION("BVC should return one extra cycle on branch to same page")
  {
    cpu.PC = 0x1070;

    set_flag(cpu.SR, Flag::V, false);
    int const cycles = BVC(cpu, TARGET);
    REQUIRE(cpu.PC == TARGET);
    REQUIRE(cycles == 1);
  }
}

TEST_CASE("BVS", "[instructions]")
{
  Address const TARGET = 0x1000;

  auto cpu = CPU{};
  cpu.PC   = 0x0100;

  SECTION("BVS should branch when overflow flag is set")
  {
    set_flag(cpu.SR, Flag::V, true);
    int const cycles = BVS(cpu, TARGET);
    REQUIRE(cpu.PC == TARGET);
    REQUIRE(cycles == 2);
  }

  SECTION("BVS should not branch when overflow flag is clear")
  {
    set_flag(cpu.SR, Flag::V, false);
    int const cycles = BVS(cpu, TARGET);
    REQUIRE(cpu.PC == 0x0100);
    REQUIRE(cycles == 0);
  }

  SECTION("BVS should return one extra cycle on branch to same page")
  {
    cpu.PC = 0x1070;

    set_flag(cpu.SR, Flag::V, true);
    int const cycles = BVS(cpu, TARGET);
    REQUIRE(cpu.PC == TARGET);
    REQUIRE(cycles == 1);
  }
}

TEST_CASE("JMP", "[instructions]")
{
  auto const TARGET = Address{0xFADE};

  auto cpu = CPU{};
  cpu.SP   = 0x0034;

  JMP(cpu, TARGET);
  REQUIRE(cpu.PC == TARGET);
}

TEST_CASE("JSR + RTS", "[instructions]")
{
  auto const TARGET = Address{0xFADE};

  auto cpu = CPU{};
  auto mem = Memory{};
  cpu.PC   = 0x1234;

  REQUIRE(cpu.SP == 0xFF);
  JSR(cpu, mem, TARGET);
  REQUIRE(cpu.SP == 0xFD);
  REQUIRE(cpu.PC == TARGET);

  RTS(cpu, mem);
  REQUIRE(cpu.SP == 0xFF);
  REQUIRE(cpu.PC == 0x1234);
}

TEST_CASE("BRK + RTI", "[instructions]")
{
  auto cpu = CPU{};
  auto mem = Memory{};

  cpu.PC = 0x1002;
  cpu.SP = 0xFF;

  set_flag(cpu.SR, Flag::N, true);
  set_flag(cpu.SR, Flag::U, false);  // So init_status is correct; U is ignored.

  Byte const init_status = cpu.SR;
  Byte pushed_status     = init_status;
  set_flag(pushed_status, Flag::B, true);
  set_flag(pushed_status, Flag::U, true);

  // Interrupt Vector for BRK
  mem.write(0xFFFE, 0x34);
  mem.write(0xFFFF, 0x12);

  BRK(cpu, mem);

  set_flag(cpu.SR, Flag::N, false);
  set_flag(cpu.SR, Flag::Z, true);

  REQUIRE(get_flag(cpu.SR, Flag::I) == true);
  REQUIRE(cpu.PC == 0x1234);
  REQUIRE(mem.read(0x01FE) == 0x03);  // lo first PC
  REQUIRE(mem.read(0x01FF) == 0x10);  // hi first PC
  REQUIRE(mem.read(0x01FD) == pushed_status);  // first SR | B | U
  REQUIRE(cpu.SP == 0xFC);

  RTI(cpu, mem);

  REQUIRE(cpu.SP == 0xFF);
  REQUIRE(cpu.PC == 0x1003);
  REQUIRE(cpu.SR == init_status);
}

TEST_CASE("BIT", "[instructions]")
{
  auto cpu = CPU{};
  cpu.AC = 0x01;
  BIT(cpu, 0b01010101);
  REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  REQUIRE(get_flag(cpu.SR, Flag::V) == true);
  REQUIRE(get_flag(cpu.SR, Flag::N) == false);
}

TEST_CASE("IRQ", "[instructions]")
{
  auto cpu = CPU{};
  auto mem = Memory{};

  SECTION("NOP if Flag::I is set")
  {
    cpu.PC = 0x1002;
    cpu.SP = 0xFF;

    set_flag(cpu.SR, Flag::I, true);
    set_flag(cpu.SR, Flag::N, true);

    Byte const init_status = cpu.SR;

    // Interrupt Vector for BRK
    mem.write(0xFFFE, 0x34);
    mem.write(0xFFFF, 0x12);

    IRQ(cpu, mem);
    REQUIRE(cpu.SP == 0xFF);
    REQUIRE(cpu.PC == 0x1002);
    REQUIRE(cpu.SR == init_status);
  }
  SECTION("Interrupt if Flag::I is not set")
  {
    cpu.PC = 0x1002;
    cpu.SP = 0xFF;

    set_flag(cpu.SR, Flag::N, true);
    set_flag(cpu.SR, Flag::I, false);

    Byte const init_status = cpu.SR;
    Byte pushed_status     = init_status;
    set_flag(pushed_status, Flag::U, true);

    // Interrupt Vector for BRK
    mem.write(0xFFFE, 0x34);
    mem.write(0xFFFF, 0x12);

    IRQ(cpu, mem);

    set_flag(cpu.SR, Flag::N, false);
    set_flag(cpu.SR, Flag::Z, true);

    REQUIRE(get_flag(cpu.SR, Flag::I) == true);
    REQUIRE(cpu.PC == 0x1234);
    REQUIRE(mem.read(0x01FE) == 0x02);           // lo first PC
    REQUIRE(mem.read(0x01FF) == 0x10);           // hi first PC
    REQUIRE(mem.read(0x01FD) == pushed_status);  // first SR | U
    REQUIRE(cpu.SP == 0xFC);
  }
}

TEST_CASE("NMI", "[instructions]")
{
  auto cpu = CPU{};
  auto mem = Memory{};

  SECTION("Interrupt regardless of Flag::I")
  {
    cpu.PC = 0x1002;
    cpu.SP = 0xFF;

    set_flag(cpu.SR, Flag::N, true);
    set_flag(cpu.SR, Flag::I, true);

    Byte const init_status = cpu.SR;
    Byte pushed_status     = init_status;
    set_flag(pushed_status, Flag::U, true);

    // Interrupt Vector for BRK
    mem.write(0xFFFA, 0x34);
    mem.write(0xFFFB, 0x12);

    NMI(cpu, mem);

    set_flag(cpu.SR, Flag::N, false);
    set_flag(cpu.SR, Flag::Z, true);

    REQUIRE(get_flag(cpu.SR, Flag::I) == true);
    REQUIRE(cpu.PC == 0x1234);
    REQUIRE(mem.read(0x01FE) == 0x02);           // lo first PC
    REQUIRE(mem.read(0x01FF) == 0x10);           // hi first PC
    REQUIRE(mem.read(0x01FD) == pushed_status);  // first SR | U
    REQUIRE(cpu.SP == 0xFC);
  }
}

TEST_CASE("reset", "[instructions]")
{
  auto cpu = CPU{};
  auto mem = Memory{};

  mem.write(0xFFFC, 0x34);
  mem.write(0xFFFD, 0x12);

  cpu.PC = 0x0F0F;
  cpu.SP = 0x77;
  cpu.SR = 0b10011011;
  cpu.AC = 0x05;
  cpu.X  = 0x55;
  cpu.Y  = 0x71;

  reset(cpu, mem);

  REQUIRE(cpu.PC == 0x1234);
  REQUIRE(cpu.SP == 0xFF);
  REQUIRE(get_flag(cpu.SR, Flag::C) == false);
  REQUIRE(get_flag(cpu.SR, Flag::Z) == false);
  REQUIRE(get_flag(cpu.SR, Flag::I) == true);
  REQUIRE(get_flag(cpu.SR, Flag::D) == false);
  REQUIRE(get_flag(cpu.SR, Flag::B) == false);
  REQUIRE(get_flag(cpu.SR, Flag::U) == true);
  REQUIRE(get_flag(cpu.SR, Flag::V) == false);
  REQUIRE(get_flag(cpu.SR, Flag::N) == false);
  REQUIRE(cpu.AC == 0x00);
  REQUIRE(cpu.X == 0x00);
  REQUIRE(cpu.Y == 0x00);
}
