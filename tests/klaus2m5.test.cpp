#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <bitset>

#include <mos6502/byte_array.hpp>
#include <mos6502/opcodes.hpp>
#include <mos6502/execute.hpp>

using namespace mos6502;

// namespace {

// auto print_cpu(std::ostream& os, CPU& cpu) -> void
// {
//   os << std::hex;
//   os << "AC: " << (int)cpu.AC << '\n';
//   os << "X:  " << (int)cpu.X << '\n';
//   os << "Y:  " << (int)cpu.Y << '\n';
//   os << "SR: " << std::bitset<8>(cpu.SR) << '\n';
//   os << "SP: " << (int)cpu.SP << '\n';
//   os << "PC: " << cpu.PC << '\n';
//   os << std::endl;
// }

// } // namespace

TEST_CASE("6502_functional_test.a65", "[klaus2m5]")
{
  constexpr auto SUCCESS_ADDR = 0x3469;

  auto bytes = load_binary("../roms/6502_functional_test.bin", 0x0000);

  constexpr auto table = create_opcode_table<decltype(bytes)>();

  auto cpu = CPU{};
  cpu.PC = 0x0400;

  auto exe_instruction = [prev = -1, &cpu, &bytes, &table]() mutable
  {
    if (prev == cpu.PC) {
      return true;
    }
    prev = cpu.PC;
    execute_next_opcode(table, cpu, bytes);
    return false;
  };

  auto exit = false;
  while (!exit) {
    exit = exe_instruction();
  }
  REQUIRE(cpu.PC == SUCCESS_ADDR);
}
