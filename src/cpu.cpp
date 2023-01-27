#include <mos6502/cpu.hpp>

#include <mos6502/byte.hpp>
#include <mos6502/cpu.hpp>
#include <mos6502/opcodes.hpp>

namespace {

using namespace mos6502;

/// Return the opcode at cpu.PC and increment cpu.PC.
[[nodiscard]]
auto read_opcode(
  CPU& cpu,
  Memory& mem
) -> Byte
{
  return mem.read(cpu.PC++);
}

}  // namespace

namespace mos6502 {

auto get_flag(
  Byte status_register,
  Flag flag
) -> bool
{
  return status_register & flag;
}

auto set_flag(
  Byte& status_register,
  Flag flag,
  bool value
) -> void
{
  if (value)
    status_register |= flag;
  else
    status_register &= ~flag;
}

auto execute_next_opcode(
  CPU& cpu,
  Memory& mem
) -> int
{
  return opcode_table[read_opcode(cpu, mem)](cpu, mem);
}

}  // namespace mos6502
