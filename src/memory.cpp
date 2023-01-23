#include <nes/memory.hpp>

#include <algorithm>

#include <nes/address.hpp>
#include <nes/byte.hpp>

namespace nes {

Memory::Memory()
{
  std::ranges::fill(ram_, 0);
  std::ranges::fill(ppu_, 0);
  std::ranges::fill(apu_, 0);
  std::ranges::fill(rom_, 0);
}

auto Memory::read(
  Address a
) const -> Byte
{
  return this->get_memory_ref(a);
}

auto Memory::write(
  Address a,
  Byte data
) -> void
{
  this->get_memory_ref(a) = data;
}

auto Memory::get_memory_ref(
  Address a
) -> Byte&
{
  return const_cast<Byte&>(
    static_cast<Memory const&>(*this).get_memory_ref(a));
}

auto Memory::get_memory_ref(
  Address a
) const -> Byte const&
{
  constexpr auto PPU_START = 0x2000;
  constexpr auto APU_START = 0x4000;
  constexpr auto ROM_START = 0x4020;
  if (a < PPU_START) {
    return ram_[a % ram_.size()];
  }
  else if (a < APU_START) {
    return ppu_[(a - PPU_START) % ppu_.size()];
  }
  else if (a < ROM_START) {
    return apu_[a - APU_START];
  }
  else {
    return rom_[a - ROM_START];
  }
}

}  // namespace nes
