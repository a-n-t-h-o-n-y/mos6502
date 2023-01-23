#include <nes/instructions.hpp>
#include <nes/word.hpp>

namespace nes {

auto ADC(
  CPU& cpu,
  Byte value
) -> void
{
  Word const result =
    static_cast<Word>(cpu.AC) +
    static_cast<Word>(value) +
    static_cast<Word>(get_flag(cpu.SR, Flag::C));

  // Carry Flag
  set_flag(
    cpu.SR,
    Flag::C,
    static_cast<bool>(result & (1 << 8))
  );

  // Zero Flag
  set_flag(
    cpu.SR,
    Flag::Z,
    (result & 0x00FF) == 0
  );

  // Signed Overflow Flag
  set_flag(
    cpu.SR,
    Flag::V,
    static_cast<bool>((
      ~(static_cast<Word>(cpu.AC) ^ static_cast<Word>(value)) &
       (static_cast<Word>(cpu.AC) ^ static_cast<Word>(result))
    ) & 0x0080)
  );

  // Negative Flag
  set_flag(
    cpu.SR,
    Flag::N,
    static_cast<bool>(result & 0x0080)
  );

  cpu.AC = result & 0x00FF;
}

auto SBC(
  CPU& cpu,
  Byte value
) -> void
{
  // A + (flip_sign(M) - 1) + C
  // A + flip_bits(M) + C
  ADC(cpu, ~value);
}

}  // namespace nes
