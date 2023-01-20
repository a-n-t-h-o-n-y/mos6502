#include <nes/memory.hpp>

#include <algorithm>
#include <cstdint>

namespace nes {

Memory::Memory()
{
  std::ranges::fill(ram_, 0);
  std::ranges::fill(ppu_, 0);
  std::ranges::fill(apu_, 0);
  std::ranges::fill(rom_, 0);
}

auto Memory::read(std::uint16_t address) -> std::uint8_t
{
  return this->get_memory_ref(address);
}

auto Memory::write(std::uint16_t address, std::uint8_t data) -> void
{
  this->get_memory_ref(address) = data;
}

auto Memory::get_memory_ref(std::uint16_t address) -> std::uint8_t&
{
  constexpr auto PPU_START = 0x2000;
  constexpr auto APU_START = 0x4000;
  constexpr auto ROM_START = 0x4020;
  if (address < PPU_START) {
    return ram_[address % ram_.size()];
  }
  else if (address < APU_START) {
    return ppu_[(address - PPU_START) % ppu_.size()];
  }
  else if (address < ROM_START) {
    return apu_[address - APU_START];
  }
  else {
    return rom_[address - ROM_START];
  }
}

}  // namespace nes
