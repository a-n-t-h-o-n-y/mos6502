#ifndef NES_ADDRESS_MODES_HPP
#define NES_ADDRESS_MODES_HPP
#include <utility>

#include <nes/address.hpp>
#include <nes/byte.hpp>
#include <nes/cpu.hpp>
#include <nes/memory.hpp>

namespace nes {

/// Implied
inline
auto IMP() -> void { return; }

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

/// Absolute - X Indexed - Returns {PageCrossed? bool, Address}
[[nodiscard]]
auto ABX(
  CPU& cpu,
  Memory const& memory
) -> std::pair<bool, Address>;

/// Absolute - Y Indexed - Returns {PageCrossed? bool, Address}
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

/// Indirect - Y Indexed
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

}  // namespace nes
#endif  // NES_ADDRESS_MODES_HPP
