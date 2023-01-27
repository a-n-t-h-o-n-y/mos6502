#ifndef MOS6502_ADDRESSING_MODE_HELPERS_HPP
#define MOS6502_ADDRESSING_MODE_HELPERS_HPP
#include <utility>

#include <mos6502/address.hpp>
#include <mos6502/byte.hpp>
#include <mos6502/cpu.hpp>
#include <mos6502/memory.hpp>

namespace mos6502::detail {

/// Absolute Mode for various offsets.
inline
auto abs_indexed(
  CPU& cpu,
  Memory auto const& memory,
  Byte index
) -> std::pair<bool, Address>
{
  Address const abs = ABS(cpu, memory);
  if (((abs + index) & 0xFF00) != (abs & 0xFF00))
    return {true, abs + index};
  else
    return {false, abs + index};
}

/// Two's complement byte to two's complement word (2 Bytes).
inline
auto sign_extend(
  Byte b
) -> Address
{
  Address word = b;
  if (word & 0x0080)
    word |= 0xFF00;
  return word;
}

}  // namespace mos6502
#endif //MOS6502_ADDRESSING_MODE_HELPERS_HPP
