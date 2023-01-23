#include <nes/cpu.hpp>

namespace nes {

auto get_flag(
  Byte status_register,
  Flag flag
) -> bool
{
  return (status_register & (1 << static_cast<Byte>(flag))) != 0;
}

auto set_flag(
  Byte& status_register,
  Flag flag,
  bool value
) -> void
{
  if (value)
    status_register |= 0x01 << static_cast<Byte>(flag);
  else
    status_register &= ~(0x01 << static_cast<Byte>(flag));
}

}
