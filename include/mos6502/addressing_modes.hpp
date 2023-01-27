#ifndef MOS6502_ADDRESSING_MODES_HPP
#define MOS6502_ADDRESSING_MODES_HPP
#include <utility>

#include <mos6502/address.hpp>
#include <mos6502/byte.hpp>
#include <mos6502/cpu.hpp>
#include <mos6502/detail/addressing_mode_helpers.hpp>
#include <mos6502/memory.hpp>

namespace mos6502 {

/// Implied
inline
auto IMP() -> void {}

/// Accumulator - Returns Byte
[[nodiscard]] inline
auto ACC(
  CPU const& cpu
) -> Byte { return cpu.AC; }

/// Absolute
[[nodiscard]]
auto ABS(
  CPU& cpu,
  Memory auto const& memory
) -> Address
{
  Address const lo = memory.read(cpu.PC++);
  Address const hi = memory.read(cpu.PC++);

  return (hi << 8) | lo;
}

/// Absolute - X Indexed - Returns {PageCrossed?, Address}
[[nodiscard]]
auto ABX(
  CPU& cpu,
  Memory auto const& memory
) -> std::pair<bool, Address>
{
  return detail::abs_indexed(cpu, memory, cpu.X);
}

/// Absolute - Y Indexed - Returns {PageCrossed?, Address}
[[nodiscard]]
auto ABY(
  CPU& cpu,
  Memory auto const& memory
) -> std::pair<bool, Address>
{
  return detail::abs_indexed(cpu, memory, cpu.Y);
}

/// Immediate - Returns Single Byte
[[nodiscard]]
auto IMM(
  CPU& cpu,
  Memory auto const& memory
) -> Byte
{
  return memory.read(cpu.PC++);
}

/// Indirect
[[nodiscard]]
auto IND(
  CPU& cpu,
  Memory auto const& memory
) -> Address
{
  Address const abs = ABS(cpu, memory);
  if ((abs & 0x00FF) == 0xFF)
    return memory.read(abs & 0xFF00) << 8 | memory.read(abs);
  else
    return memory.read(abs + 1) << 8 | memory.read(abs);
}

/// Indirect - X Indexed
[[nodiscard]]
auto IZX(
  CPU& cpu,
  Memory auto const& memory
) -> Address
{
  Address const adr = memory.read(cpu.PC++) + cpu.X;
  Address const lo  = memory.read(adr);
  Address const hi  = memory.read(adr + 1);
  return (hi << 8) | lo;
}

/// Indirect - Y Indexed - Returns {PageCrossed?, Address}
[[nodiscard]]
auto IZY(
  CPU& cpu,
  Memory auto const& memory
) -> std::pair<bool, Address>
{
  Address const zp = memory.read(cpu.PC++);
  Address const lo = memory.read(zp);
  Address const hi = memory.read(zp + 1);
  Address const r  = static_cast<Address>(((hi << 8) | lo) + cpu.Y);
  if ((hi << 8) != (r & 0xFF00))
    return {true, r};
  else
    return {false, r};
}

/// Relative
[[nodiscard]]
auto REL(
  CPU& cpu,
  Memory auto const& memory
) -> Address
{
  Byte const bb = memory.read(cpu.PC++);
  return cpu.PC + detail::sign_extend(bb);
}

/// Zero Page
[[nodiscard]]
auto ZP0(
  CPU& cpu,
  Memory auto const& memory
) -> Address
{
  return Address{memory.read(cpu.PC++)};
}

/// Zero Page - X Indexed
[[nodiscard]]
auto ZPX(
  CPU& cpu,
  Memory auto const& memory
) -> Address {
  return (memory.read(cpu.PC++) + cpu.X) & 0x00FF;
}

/// Zero Page - Y Indexed
[[nodiscard]]
auto ZPY(
  CPU& cpu,
  Memory auto const& memory
) -> Address
{
  return (memory.read(cpu.PC++) + cpu.Y) & 0x00FF;
}

}  // namespace mos6502
#endif  // MOS6502_ADDRESSING_MODES_HPP
