#ifndef MOS6502_DETAIL_BCD_HPP
#define MOS6502_DETAIL_BCD_HPP
#include <mos6502/byte.hpp>
#include <mos6502/word.hpp>

namespace mos6502::detail {

/// Convert Binary Coded Decimal to Uncoded Binary.
/** Does not check that \p b is a valid BCD value. */
inline
auto BCD_to_BIN(
  Byte b
) -> Byte
{
  return (b & 0x0F) + Byte(((b >> 4) & 0x0F) * 10);
}

/// Set a nibble to a given value, nibble_index starts at zero, from LSN.
template <typename Int_t>
auto set_nibble(
  Int_t original,
  Byte nibble_value,
  Byte nibble_index
) -> Int_t
{
  auto const nibble_offset = nibble_index * 4;

  // clear nibble before setting new value
  original &= Int_t(~(0x0F << nibble_offset));

  return Int_t(original | nibble_value << nibble_offset);
}

inline
auto BIN_to_BCD(
  Byte b
) -> Byte
{
  Word workspace = b;
  for (auto i = 0; i < 8; ++i) {
    auto const lo = (workspace >> 8) & 0x0F;
    auto const hi = (workspace >> 12) & 0x0F;
    if (lo > 4)
      workspace = set_nibble(workspace, Byte(lo + 3), 2);
    if (hi > 4)
      workspace = set_nibble(workspace, Byte(hi + 3), 3);
    workspace <<= 1;
  }
  return Byte((workspace & 0xFF00) >> 8);
}

}  // namespace mos6502::detail
#endif //MOS6502_DETAIL_BCD_HPP
