#ifndef NES_MEMORY_HPP
#define NES_MEMORY_HPP
#include <array>
#include <cstdint>

namespace nes {

class Memory {
 public:
  Memory();

 public:
  [[nodiscard]]  //
  auto
  read(std::uint16_t address) -> std::uint8_t;

  auto write(std::uint16_t address, std::uint8_t data) -> void;

 private:
  auto get_memory_ref(std::uint16_t address) -> std::uint8_t&;

 private:
  // Memory regions
  std::array<std::uint8_t, 0x800> ram_;   // 2KB  of RAM
  std::array<std::uint8_t, 0x0008> ppu_;  // 8B   of PPU memory
  std::array<std::uint8_t, 0x0020> apu_;  // 32B  of APU memory
  std::array<std::uint8_t, 0xBFE0> rom_;  // 49KB of ROM memory
};

}  // namespace nes
#endif  // NES_MEMORY_HPP
