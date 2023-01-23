#ifndef NES_MEMORY_HPP
#define NES_MEMORY_HPP
#include <array>

#include <nes/address.hpp>
#include <nes/byte.hpp>

namespace nes {

class Memory {
 public:
  Memory();

 public:
  [[nodiscard]]
  auto read(
    Address a
  ) const -> Byte;

  auto write(
    Address a,
    Byte data
  ) -> void;

 private:
  auto get_memory_ref(
    Address a
  ) -> Byte&;

  auto get_memory_ref(
    Address a
  ) const -> Byte const&;

 private:
  // Memory regions
  std::array<Byte, 0x800>  ram_;  // 2KB  of RAM
  std::array<Byte, 0x0008> ppu_;  // 8B   of PPU memory
  std::array<Byte, 0x0020> apu_;  // 32B  of APU memory
  std::array<Byte, 0xBFE0> rom_;  // 49KB of ROM memory
};

}  // namespace nes
#endif  // NES_MEMORY_HPP
