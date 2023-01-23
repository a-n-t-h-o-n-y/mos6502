#ifndef NES_CPU_HPP
#define NES_CPU_HPP
#include <nes/address.hpp>
#include <nes/byte.hpp>

namespace nes {

/// Status Flag Bit Positions
enum class Flag : Byte {
  C = 0,  // Carry
  Z = 1,  // Zero
  I = 2,  // Interrupt Disable
  D = 3,  // Decimal
  B = 4,  // Break
  V = 6,  // Overflow
  N = 7,  // Negative
};

/// 6502 CPU State
struct CPU {
  // Accumulator Register
  Byte AC = 0x00;

  // X & Y Registers
  Byte X = 0x00;
  Byte Y = 0x00;

  // Status Register
  Byte SR = 0x00;

  // Stack Pointer
  Byte SP = 0xFF;

  // Program Counter
  Address PC = 0x0000;
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

}  // namespace nes
#endif  // NES_CPU_HPP
