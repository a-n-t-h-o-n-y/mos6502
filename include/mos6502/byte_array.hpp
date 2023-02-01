#ifndef MOS6502_BYTE_ARRAY_HPP
#define MOS6502_BYTE_ARRAY_HPP
#include <array>
#include <climits>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>

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

/// Load a compiled binary ROM into \p offset into a ByteArray.
[[nodiscard]] inline
auto load_binary(
  std::string const& filename,
  Address offset = 0x0000
) -> ByteArray
{
  auto out  = ByteArray{};
  auto file = std::ifstream{filename, std::ios::binary};
  if (!file) {
    throw std::runtime_error("Unable to open file: " + filename);
  }
  file.seekg(0, file.end);
  auto const file_size = file.tellg();
  auto const size = file_size + (decltype(file_size))offset;
  if (size > 0x10000) {
    throw std::runtime_error(
      "File size + offset exceeds memory size (" +
      std::to_string(size) +
      ")."
    );
  }
  file.seekg(0, file.beg);
  file.read(
    reinterpret_cast<char*>(&(*std::next(out.begin(), offset))),
    file_size
  );
  return out;
}

}  // namespace mos6502
#endif  // MOS6502_BYTE_ARRAY_HPP
