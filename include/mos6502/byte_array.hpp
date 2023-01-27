#ifndef MOS6502_BYTE_ARRAY_HPP
#define MOS6502_BYTE_ARRAY_HPP
#include <array>
#include <climits>

#include <mos6502/address.hpp>
#include <mos6502/byte.hpp>

namespace mos6502 {

/// 64KB Byte Array. Implements Memory Concept. No Initialization Performed.
class ByteArray {
 public:
  /// Return a single Byte at Address \p at.
  [[nodiscard]]
  auto read(
    Address at
  ) const -> Byte
  {
    return bytes_[at];
  }

  /// Write \p val to Address \p at.
  auto write(
    Address at,
    Byte val
  ) -> void
  {
    bytes_[at] = val;
  }

  // Iterators
  constexpr auto cbegin() const { return bytes_.begin(); }
  constexpr auto begin() const { return bytes_.begin(); }
  constexpr auto begin() { return bytes_.begin(); }

  constexpr auto cend() const { return bytes_.end(); }
  constexpr auto end() const { return bytes_.end(); }
  constexpr auto end() { return bytes_.end(); }

 private:
  static constexpr
  auto SIZE = 1 << (sizeof(Address) * CHAR_BIT) ;
  static_assert(SIZE == 65'536);

  std::array<Byte, SIZE> bytes_;
};

}  // namespace mos6502
#endif  // MOS6502_BYTE_ARRAY_HPP
