#include <catch2/catch_test_macros.hpp>

#include <nes/cpu.hpp>
#include <nes/memory.hpp>
#include <nes/opcodes.hpp>

using namespace nes;

TEST_CASE("ADC", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  nes::opcode_table[0x69](cpu, memory);
  nes::opcode_table[0x65](cpu, memory);
  nes::opcode_table[0x75](cpu, memory);
  nes::opcode_table[0x6D](cpu, memory);
  nes::opcode_table[0x7D](cpu, memory);
  nes::opcode_table[0x79](cpu, memory);
  nes::opcode_table[0x61](cpu, memory);
  nes::opcode_table[0x71](cpu, memory);
}

TEST_CASE("SBC", "[opcodes]")
{
  auto cpu    = CPU{};
  auto memory = Memory{};

  nes::opcode_table[0xE9](cpu, memory);
  nes::opcode_table[0xE5](cpu, memory);
  nes::opcode_table[0xF5](cpu, memory);
  nes::opcode_table[0xED](cpu, memory);
  nes::opcode_table[0xFD](cpu, memory);
  nes::opcode_table[0xF9](cpu, memory);
  nes::opcode_table[0xE1](cpu, memory);
  nes::opcode_table[0xF1](cpu, memory);
}
