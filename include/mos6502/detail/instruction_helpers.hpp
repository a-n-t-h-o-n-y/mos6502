#ifndef MOS6502_DETAIL_INSTRUCTION_HELPERS_HPP
#define MOS6502_DETAIL_INSTRUCTION_HELPERS_HPP
#include <mos6502/address.hpp>
#include <mos6502/byte.hpp>
#include <mos6502/cpu.hpp>
#include <mos6502/memory.hpp>
#include <mos6502/word.hpp>

namespace mos6502::detail {

/// Set the Negative Flag
inline
auto set_flag_N(
  CPU& cpu,
  Byte value
) -> void
{
  set_flag(
    cpu.SR,
    Flag::N,
    static_cast<bool>(value & 0x80)
  );
}

/// Set the Zero Flag
inline
auto set_flag_Z(
  CPU& cpu,
  Byte value
) -> void
{
  set_flag(
    cpu.SR,
    Flag::Z,
    value == 0
  );
}

/// Set the Carry Flag
inline
auto set_flag_C(
  CPU& cpu,
  Word value
) -> void
{
  set_flag(
    cpu.SR,
    Flag::C,
    static_cast<bool>(value & (0x0001 << 8))
  );
}

/// Pull single Byte from stack pointer location and increment stack pointer.
[[nodiscard]] inline
auto stack_pull(
  CPU& cpu,
  Memory auto const& mem
) -> Byte
{
  cpu.SP += 1;
  return mem.read(0x0100 + cpu.SP);
}

/// Push single Byte to the current stack pointer, then decrement stack pointer.
inline
auto stack_push(
  CPU& cpu,
  Memory auto& mem,
  Byte value
) -> void
{
  mem.write(0x0100 + cpu.SP, value);
  cpu.SP -= 1;
}

/// Push the PC onto the stack.
inline
auto push_PC(
  CPU& cpu,
  Memory auto& mem
) -> void
{
  auto const previous = cpu.PC;

  Byte const hi = Byte((previous >> 8) & 0x00FF);
  Byte const lo = Byte(previous & 0x00FF);
  stack_push(cpu, mem, hi);
  stack_push(cpu, mem, lo);
}

/// Pull the PC off the stack.
[[nodiscard]] inline
auto pull_PC(
  CPU& cpu,
  Memory auto const& mem
) -> Address
{
  Byte const lo = stack_pull(cpu, mem);
  Byte const hi = stack_pull(cpu, mem);
  return (hi << 8) | lo;
}

/// Shift Left One Bit. Carry is set to MSB before shift.
inline
auto ASL(
  CPU& cpu,
  Byte val
) -> Byte
{
  // Set carry flag before MSB is shifted out of existence.
  set_flag(
    cpu.SR,
    Flag::C,
    val & (0x01 << 7)
  );
  Byte const r = Byte(val << 1);
  set_flag_N(cpu, r);
  set_flag_Z(cpu, r);
  return r;
}

/// Shift Right One Bit. Carry is set to LSB before shift.
inline
auto LSR(
  CPU& cpu,
  Byte val
) -> Byte
{
  set_flag(
    cpu.SR,
    Flag::C,
    static_cast<bool>(val & 0x01)
  );
  Byte const r = val >> 1;
  set_flag_N(cpu, r);
  set_flag_Z(cpu, r);
  return r;
}

/// Rotate Left One Bit. MSB set as carry, original carry placed in LSB.
inline
auto ROL(
  CPU& cpu,
  Byte val
) -> Byte
{
  // Important to get the carry flag before ASL() overwrites it.
  Byte const c = get_flag(cpu.SR, Flag::C);
  Byte const r = ASL(cpu, val) | c;
  set_flag_Z(cpu, r);
  return r;
}

/// Rotate Right One Bit. LSB set as carry, original carry placed in MSB.
inline
auto ROR(
  CPU& cpu,
  Byte val
) -> Byte
{
  // Important to get the carry flag before LSR() overwrites it.
  Byte const c = get_flag(cpu.SR, Flag::C);
  Byte const r = Byte(LSR(cpu, val) | (c << 7));
  set_flag_N(cpu, r);
  set_flag_Z(cpu, r);
  return r;
}

/// Updates CPU Status Flags based on result of reg - val;
inline
auto compare(
  CPU& cpu,
  Byte reg,
  Byte val
) -> void
{
  Byte const result = reg - val;
  set_flag_Z(cpu, result);
  set_flag_N(cpu, result);
  set_flag(cpu.SR, Flag::C, reg >= val);
}

/**
 * @brief Branch if condition is met.
 * @param condition - the condition to check
 * @param cpu - reference to the CPU object
 * @param address - the address to jump to if condition is true
 * @return int - number of extra cycles used (in range [0-2])
 */
[[nodiscard]] inline
auto branch_if(
  bool condition,
  CPU& cpu,
  Address address
) -> int
{
  if (condition) {
    bool const page_change = (address & 0xFF00) != (cpu.PC & 0xFF00);
    cpu.PC = address;
    return page_change ? 2 : 1;
  }
  return 0;
}

/// Reads two bytes in lo byte, hi byte order and returns as Address.
[[nodiscard]] inline
auto read_address(
  Memory auto const& mem,
  Address at
) -> Address
{
  return Address{mem.read(at)} | (Address{mem.read(at + 1)} << 8);
}

/// Handles interrupts, pushes PC and SR to stack, sets flags, jumps to Address.
inline
auto interrupt(
  CPU& cpu,
  Memory auto& mem,
  Address to,
  bool is_software
) -> void
{
  // PC
  push_PC(cpu, mem);

  // Status Register
  Byte status = cpu.SR;
  set_flag(status, Flag::B, is_software);
  set_flag(status, Flag::U, true);
  stack_push(cpu, mem, status);

  // Set Interrupt Flag
  set_flag(cpu.SR, Flag::I, true);

  // Jump
  cpu.PC = read_address(mem, to);
}

/// ADC instruction for binary values only, not BCD.
inline
auto ADC_BIN(
  CPU& cpu,
  Byte value
) -> void
{
  Word const r = static_cast<Word>(
    cpu.AC + value + static_cast<Word>(get_flag(cpu.SR, Flag::C)));

  // Carry Flag
  detail::set_flag_C(cpu, r);

  // Zero Flag
  detail::set_flag_Z(cpu, static_cast<Byte>(r));

  // Signed Overflow Flag
  set_flag(
    cpu.SR,
    Flag::V,
    static_cast<bool>((
      ~(static_cast<Word>(cpu.AC) ^ static_cast<Word>(value)) &
       (static_cast<Word>(cpu.AC) ^ static_cast<Word>(r))
    ) & 0x0080)
  );

  // Negative Flag
  detail::set_flag_N(cpu, static_cast<Byte>(r));

  cpu.AC = static_cast<Byte>(r);
}

}  // namespace mos6502::detail
#endif //MOS6502_DETAIL_INSTRUCTION_HELPERS_HPP
