#include <mos6502/cpu.hpp>

#include <mos6502/byte.hpp>

namespace mos6502 {

auto get_flag(
  Byte status_register,
  Flag flag
) -> bool
{
  return status_register & flag;
}

auto set_flag(
  Byte& status_register,
  Flag flag,
  bool value
) -> void
{
  if (value)
    status_register |= flag;
  else
    status_register &= ~flag;
}

}  // namespace mos6502
