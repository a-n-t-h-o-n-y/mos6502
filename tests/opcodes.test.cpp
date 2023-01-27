#include <catch2/catch_test_macros.hpp>

#include <mos6502/cpu.hpp>
#include <mos6502/opcodes.hpp>

using namespace mos6502;

TEST_CASE("ADC", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x69](cpu, memory);
  opcode_table[0x65](cpu, memory);
  opcode_table[0x75](cpu, memory);
  opcode_table[0x6D](cpu, memory);
  opcode_table[0x7D](cpu, memory);
  opcode_table[0x79](cpu, memory);
  opcode_table[0x61](cpu, memory);
  opcode_table[0x71](cpu, memory);
}

TEST_CASE("SBC", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0xE9](cpu, memory);
  opcode_table[0xE5](cpu, memory);
  opcode_table[0xF5](cpu, memory);
  opcode_table[0xED](cpu, memory);
  opcode_table[0xFD](cpu, memory);
  opcode_table[0xF9](cpu, memory);
  opcode_table[0xE1](cpu, memory);
  opcode_table[0xF1](cpu, memory);
}

TEST_CASE("LDA", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0xA9](cpu, memory);
  opcode_table[0xA5](cpu, memory);
  opcode_table[0xB5](cpu, memory);
  opcode_table[0xAD](cpu, memory);
  opcode_table[0xBD](cpu, memory);
  opcode_table[0xB9](cpu, memory);
  opcode_table[0xA1](cpu, memory);
  opcode_table[0xB1](cpu, memory);
}

TEST_CASE("LDX", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0xA2](cpu, memory);
  opcode_table[0xA6](cpu, memory);
  opcode_table[0xB6](cpu, memory);
  opcode_table[0xAE](cpu, memory);
  opcode_table[0xBE](cpu, memory);
}

TEST_CASE("LDY", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0xA0](cpu, memory);
  opcode_table[0xA4](cpu, memory);
  opcode_table[0xB4](cpu, memory);
  opcode_table[0xAC](cpu, memory);
  opcode_table[0xBC](cpu, memory);
}

TEST_CASE("STA", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  // TODO require return value to be cycles.. except for variable ones...?
  opcode_table[0x85](cpu, memory);
  opcode_table[0x95](cpu, memory);
  opcode_table[0x8D](cpu, memory);
  opcode_table[0x9D](cpu, memory);
  opcode_table[0x99](cpu, memory);
  opcode_table[0x81](cpu, memory);
  opcode_table[0x91](cpu, memory);
}

TEST_CASE("STX", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x86](cpu, memory);
  opcode_table[0x96](cpu, memory);
  opcode_table[0x8E](cpu, memory);
}

TEST_CASE("STY", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x84](cpu, memory);
  opcode_table[0x94](cpu, memory);
  opcode_table[0x8C](cpu, memory);
}

TEST_CASE("TAX", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0xAA](cpu, memory);
}

TEST_CASE("TAY", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0xA8](cpu, memory);
}

TEST_CASE("TSX", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0xBA](cpu, memory);
}

TEST_CASE("TXA", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x8A](cpu, memory);
}

TEST_CASE("TXS", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x9A](cpu, memory);
}

TEST_CASE("TYA", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x98](cpu, memory);
}

TEST_CASE("PHA", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x48](cpu, memory);
}

TEST_CASE("PHP", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x08](cpu, memory);
}

TEST_CASE("PLA", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x68](cpu, memory);
}

TEST_CASE("PLP", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x28](cpu, memory);
}

TEST_CASE("DEC", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0xC6](cpu, memory);
  opcode_table[0xD6](cpu, memory);
  opcode_table[0xCE](cpu, memory);
  opcode_table[0xDE](cpu, memory);
}

TEST_CASE("DEX", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0xCA](cpu, memory);
}

TEST_CASE("DEY", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x88](cpu, memory);
}

TEST_CASE("INC", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0xE6](cpu, memory);
  opcode_table[0xF6](cpu, memory);
  opcode_table[0xEE](cpu, memory);
  opcode_table[0xFE](cpu, memory);
}

TEST_CASE("INX", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0xE8](cpu, memory);
}

TEST_CASE("INY", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0xC8](cpu, memory);
}

TEST_CASE("AND", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x29](cpu, memory);
  opcode_table[0x25](cpu, memory);
  opcode_table[0x35](cpu, memory);
  opcode_table[0x2D](cpu, memory);
  opcode_table[0x3D](cpu, memory);
  opcode_table[0x39](cpu, memory);
  opcode_table[0x21](cpu, memory);
  opcode_table[0x31](cpu, memory);
}

TEST_CASE("EOR", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x49](cpu, memory);
  opcode_table[0x45](cpu, memory);
  opcode_table[0x55](cpu, memory);
  opcode_table[0x4D](cpu, memory);
  opcode_table[0x5D](cpu, memory);
  opcode_table[0x59](cpu, memory);
  opcode_table[0x41](cpu, memory);
  opcode_table[0x51](cpu, memory);
}

TEST_CASE("ORA", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x09](cpu, memory);
  opcode_table[0x05](cpu, memory);
  opcode_table[0x15](cpu, memory);
  opcode_table[0x0D](cpu, memory);
  opcode_table[0x1D](cpu, memory);
  opcode_table[0x19](cpu, memory);
  opcode_table[0x01](cpu, memory);
  opcode_table[0x11](cpu, memory);
}

TEST_CASE("ASL", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x0A](cpu, memory);
  opcode_table[0x06](cpu, memory);
  opcode_table[0x16](cpu, memory);
  opcode_table[0x0E](cpu, memory);
  opcode_table[0x1E](cpu, memory);
}

TEST_CASE("LSR", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x4A](cpu, memory);
  opcode_table[0x46](cpu, memory);
  opcode_table[0x56](cpu, memory);
  opcode_table[0x4E](cpu, memory);
  opcode_table[0x5E](cpu, memory);
}

TEST_CASE("ROL", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x2A](cpu, memory);
  opcode_table[0x26](cpu, memory);
  opcode_table[0x36](cpu, memory);
  opcode_table[0x2E](cpu, memory);
  opcode_table[0x3E](cpu, memory);
}

TEST_CASE("ROR", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x6A](cpu, memory);
  opcode_table[0x66](cpu, memory);
  opcode_table[0x76](cpu, memory);
  opcode_table[0x6E](cpu, memory);
  opcode_table[0x7E](cpu, memory);
}

TEST_CASE("Flag Instructions", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x18](cpu, memory);
  opcode_table[0xD8](cpu, memory);
  opcode_table[0x58](cpu, memory);
  opcode_table[0xB8](cpu, memory);
  opcode_table[0x38](cpu, memory);
  opcode_table[0xF8](cpu, memory);
  opcode_table[0x78](cpu, memory);
}

TEST_CASE("CMP", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0xC9](cpu, memory);
  opcode_table[0xC5](cpu, memory);
  opcode_table[0xD5](cpu, memory);
  opcode_table[0xCD](cpu, memory);
  opcode_table[0xDD](cpu, memory);
  opcode_table[0xD9](cpu, memory);
  opcode_table[0xC1](cpu, memory);
  opcode_table[0xD1](cpu, memory);
}

TEST_CASE("CPX", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0xE0](cpu, memory);
  opcode_table[0xE4](cpu, memory);
  opcode_table[0xEC](cpu, memory);
}

TEST_CASE("CPY", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0xC0](cpu, memory);
  opcode_table[0xC4](cpu, memory);
  opcode_table[0xCC](cpu, memory);
}

TEST_CASE("Conditional Branches", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x90](cpu, memory);
  opcode_table[0xB0](cpu, memory);
  opcode_table[0xF0](cpu, memory);
  opcode_table[0x30](cpu, memory);
  opcode_table[0xD0](cpu, memory);
  opcode_table[0x10](cpu, memory);
  opcode_table[0x50](cpu, memory);
  opcode_table[0x70](cpu, memory);
}

TEST_CASE("Jumps + Subroutines", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x4C](cpu, memory);
  opcode_table[0x6C](cpu, memory);
  opcode_table[0x20](cpu, memory);
  opcode_table[0x60](cpu, memory);
}

TEST_CASE("BRK + RTI", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x00](cpu, memory);
  opcode_table[0x40](cpu, memory);
}

TEST_CASE("BIT", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0x24](cpu, memory);
  opcode_table[0x2C](cpu, memory);
}

TEST_CASE("NOP", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  opcode_table[0xEA](cpu, memory);
}
