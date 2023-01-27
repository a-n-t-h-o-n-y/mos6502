#ifndef MOS6502_CPU_HPP
#define MOS6502_CPU_HPP
#include <mos6502/address.hpp>
#include <mos6502/byte.hpp>

namespace mos6502 {

/// Status Flag Bit Positions
enum Flag : Byte {
  C = (1 << 0),  // Carry
  Z = (1 << 1),  // Zero
  I = (1 << 2),  // Interrupt Disable
  D = (1 << 3),  // Decimal
  B = (1 << 4),  // Break
  U = (1 << 5),  // Unused
  V = (1 << 6),  // Overflow
  N = (1 << 7),  // Negative
};

/// Get status register flag.
[[nodiscard]]
auto get_flag(
  Byte status_register,
  Flag flag
) -> bool;

/// Set status register flag.
auto set_flag(
  Byte& status_register,
  Flag flag,
  bool value
) -> void;

/// 6502 CPU State
struct CPU {
  // Accumulator Register
  Byte AC = 0x00;

  // X & Y Registers
  Byte X = 0x00;
  Byte Y = 0x00;

  // Status Register
  Byte SR = 0b00100100;  // I and U Flags Set

  // Stack Pointer
  Byte SP = 0xFF;

  // Program Counter
  Address PC = 0x0000;
};

}  // namespace mos6502
#endif  // MOS6502_CPU_HPP
