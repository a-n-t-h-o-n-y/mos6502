#ifndef MOS6502_ADDRESSING_MODES_HPP
#define MOS6502_ADDRESSING_MODES_HPP
#include <utility>

#include <mos6502/address.hpp>
#include <mos6502/byte.hpp>
#include <mos6502/cpu.hpp>

namespace mos6502 {

/// Implied
inline
auto IMP() -> void {}

/// Accumulator - Returns Byte
[[nodiscard]]
auto ACC(
  CPU const& cpu
) -> Byte;

/// Absolute
[[nodiscard]]
auto ABS(
  CPU& cpu,
  Memory const& memory
) -> Address;

/// Absolute - X Indexed - Returns {PageCrossed?, Address}
[[nodiscard]]
auto ABX(
  CPU& cpu,
  Memory const& memory
) -> std::pair<bool, Address>;

/// Absolute - Y Indexed - Returns {PageCrossed?, Address}
[[nodiscard]]
auto ABY(
  CPU& cpu,
  Memory const& memory
) -> std::pair<bool, Address>;

/// Immediate - Returns Single Byte
[[nodiscard]]
auto IMM(
  CPU& cpu,
  Memory const& memory
) -> Byte;

/// Indirect
[[nodiscard]]
auto IND(
  CPU& cpu,
  Memory const& memory
) -> Address;

/// Indirect - X Indexed
[[nodiscard]]
auto IZX(
  CPU& cpu,
  Memory const& memory
) -> Address;

/// Indirect - Y Indexed - Returns {PageCrossed?, Address}
[[nodiscard]]
auto IZY(
  CPU& cpu,
  Memory const& memory
) -> std::pair<bool, Address>;

/// Relative
[[nodiscard]]
auto REL(
  CPU& cpu,
  Memory const& memory
) -> Address;

/// Zero Page
[[nodiscard]]
auto ZP0(
  CPU& cpu,
  Memory const& memory
) -> Address;

/// Zero Page - X Indexed
[[nodiscard]]
auto ZPX(
  CPU& cpu,
  Memory const& memory
) -> Address;

/// Zero Page - Y Indexed
[[nodiscard]]
auto ZPY(
  CPU& cpu,
  Memory const& memory
) -> Address;

}  // namespace mos6502
#endif  // MOS6502_ADDRESSING_MODES_HPP
