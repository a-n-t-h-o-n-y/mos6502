#include <mos6502/instructions.hpp>

#include <mos6502/detail/bcd.hpp>
#include <mos6502/detail/instruction_helpers.hpp>
#include <mos6502/memory.hpp>
#include <mos6502/word.hpp>

namespace mos6502 {

auto ADC(
  CPU& cpu,
  Byte value
) -> void
{
  if (get_flag(cpu.SR, Flag::D)) {
    value  = detail::BCD_to_BIN(value);
    cpu.AC = detail::BCD_to_BIN(cpu.AC);

    auto const ac    = cpu.AC;
    auto const carry = get_flag(cpu.SR, Flag::C) ? 1 : 0;
    detail::ADC_BIN(cpu, value);

    set_flag(cpu.SR, Flag::C, (value + carry + ac) > 99);
    cpu.AC = detail::BIN_to_BCD(cpu.AC);
  } else {
    detail::ADC_BIN(cpu, value);
  }
}

auto SBC(
  CPU& cpu,
  Byte value
) -> void
{
  if (get_flag(cpu.SR, Flag::D)) {
    value  = detail::BCD_to_BIN(value);
    cpu.AC = detail::BCD_to_BIN(cpu.AC);

    auto const ac    = cpu.AC;
    auto const carry = get_flag(cpu.SR, Flag::C) ? 1 : 0;

    detail::ADC_BIN(cpu, ~value);
    // If result is negative
    if (cpu.AC & (0x01 << 7)) {
      cpu.AC += 100;
    }
    cpu.AC = detail::BIN_to_BCD(cpu.AC);

    // Set carry if result is zero or positive
    set_flag(cpu.SR, Flag::C, (ac - (carry ? 0 : 1) - value) >= 0);

  } else {
    // A + (flip_sign(M) - 1) + C
    // A + flip_bits(M) + C
    detail::ADC_BIN(cpu, ~value);
  }
}

auto LDA(
  CPU& cpu,
  Byte value
) -> void
{
  cpu.AC = value;
  detail::set_flag_N(cpu, value);
  detail::set_flag_Z(cpu, value);
}

auto LDX(
  CPU& cpu,
  Byte value
) -> void
{
  cpu.X = value;
  detail::set_flag_N(cpu, value);
  detail::set_flag_Z(cpu, value);
}

auto LDY(
  CPU& cpu,
  Byte value
) -> void
{
  cpu.Y = value;
  detail::set_flag_N(cpu, value);
  detail::set_flag_Z(cpu, value);
}

auto TAX(
  CPU& cpu
) -> void
{
  cpu.X = cpu.AC;
  detail::set_flag_N(cpu, cpu.X);
  detail::set_flag_Z(cpu, cpu.X);
}

auto TAY(
  CPU& cpu
) -> void
{
  cpu.Y = cpu.AC;
  detail::set_flag_N(cpu, cpu.Y);
  detail::set_flag_Z(cpu, cpu.Y);
}

auto TSX(
  CPU& cpu
) -> void
{
  cpu.X = cpu.SP;
  detail::set_flag_N(cpu, cpu.X);
  detail::set_flag_Z(cpu, cpu.X);
}

auto TXA(
  CPU& cpu
) -> void
{
  cpu.AC = cpu.X;
  detail::set_flag_N(cpu, cpu.AC);
  detail::set_flag_Z(cpu, cpu.AC);
}

auto TXS(
  CPU& cpu
) -> void
{
  cpu.SP = cpu.X;
}

auto TYA(
  CPU& cpu
) -> void
{
  cpu.AC = cpu.Y;
  detail::set_flag_N(cpu, cpu.AC);
  detail::set_flag_Z(cpu, cpu.AC);
}

auto DEX(
  CPU& cpu
) -> void
{
  cpu.X -= 1;
  detail::set_flag_N(cpu, cpu.X);
  detail::set_flag_Z(cpu, cpu.X);
}

auto DEY(
  CPU& cpu
) -> void
{
  cpu.Y -= 1;
  detail::set_flag_N(cpu, cpu.Y);
  detail::set_flag_Z(cpu, cpu.Y);
}

auto INX(
  CPU& cpu
) -> void
{
  cpu.X += 1;
  detail::set_flag_N(cpu, cpu.X);
  detail::set_flag_Z(cpu, cpu.X);
}

auto INY(
  CPU& cpu
) -> void
{
  cpu.Y += 1;
  detail::set_flag_N(cpu, cpu.Y);
  detail::set_flag_Z(cpu, cpu.Y);
}

auto AND(
  CPU& cpu,
  Byte value
) -> void
{
  cpu.AC &= value;
  detail::set_flag_N(cpu, cpu.AC);
  detail::set_flag_Z(cpu, cpu.AC);
}

auto EOR(
  CPU& cpu,
  Byte value
) -> void
{
  cpu.AC ^= value;
  detail::set_flag_N(cpu, cpu.AC);
  detail::set_flag_Z(cpu, cpu.AC);
}

auto ORA(
  CPU& cpu,
  Byte value
) -> void
{
  cpu.AC |= value;
  detail::set_flag_N(cpu, cpu.AC);
  detail::set_flag_Z(cpu, cpu.AC);
}

auto ASL_ACC(
  CPU& cpu,
  Byte val
) -> void
{
  cpu.AC = detail::ASL(cpu, val);
}

auto LSR_ACC(
  CPU& cpu,
  Byte val
) -> void
{
  cpu.AC = detail::LSR(cpu, val);
}

auto ROL_ACC(
  CPU& cpu,
  Byte val
) -> void
{
  cpu.AC = detail::ROL(cpu, val);
}

auto ROR_ACC(
  CPU& cpu,
  Byte val
) -> void
{
  cpu.AC = detail::ROR(cpu, val);
}

auto CLC(
  CPU& cpu
) -> void
{
  set_flag(cpu.SR, Flag::C, false);
}

auto CLD(
  CPU& cpu
) -> void
{
  set_flag(cpu.SR, Flag::D, false);
}

auto CLI(
  CPU& cpu
) -> void
{
  set_flag(cpu.SR, Flag::I, false);
}

auto CLV(
  CPU& cpu
) -> void
{
  set_flag(cpu.SR, Flag::V, false);
}

auto SEC(
  CPU& cpu
) -> void
{
  set_flag(cpu.SR, Flag::C, true);
}

auto SED(
  CPU& cpu
) -> void
{
  set_flag(cpu.SR, Flag::D, true);
}

auto SEI(
  CPU& cpu
) -> void
{
  set_flag(cpu.SR, Flag::I, true);
}

auto CMP(
  CPU& cpu,
  Byte val
) -> void
{
  detail::compare(cpu, cpu.AC, val);
}

auto CPX(
  CPU& cpu,
  Byte val
) -> void
{
  detail::compare(cpu, cpu.X, val);
}

auto CPY(
  CPU& cpu,
  Byte val
) -> void
{
  detail::compare(cpu, cpu.Y, val);
}

auto BCC(
  CPU& cpu,
  Address to
) -> int
{
  return detail::branch_if(
    get_flag(cpu.SR, Flag::C) == false,
    cpu,
    to
  );
}

auto BCS(
  CPU& cpu,
  Address to
) -> int
{
  return detail::branch_if(
    get_flag(cpu.SR, Flag::C) == true,
    cpu,
    to
  );
}

auto BEQ(
  CPU& cpu,
  Address to
) -> int
{
  return detail::branch_if(
    get_flag(cpu.SR, Flag::Z) == true,
    cpu,
    to
  );
}

auto BMI(
  CPU& cpu,
  Address to
) -> int
{
  return detail::branch_if(
    get_flag(cpu.SR, Flag::N) == true,
    cpu,
    to
  );
}

auto BNE(
  CPU& cpu,
  Address to
) -> int
{
  return detail::branch_if(
    get_flag(cpu.SR, Flag::Z) == false,
    cpu,
    to
  );
}

auto BPL(
  CPU& cpu,
  Address to
) -> int
{
  return detail::branch_if(
    get_flag(cpu.SR, Flag::N) == false,
    cpu,
    to
  );
}

auto BVC(
  CPU& cpu,
  Address to
) -> int
{
  return detail::branch_if(
    get_flag(cpu.SR, Flag::V) == false,
    cpu,
    to
  );
}

auto BVS(
  CPU& cpu,
  Address to
) -> int
{
  return detail::branch_if(
    get_flag(cpu.SR, Flag::V) == true,
    cpu,
    to
  );
}

auto JMP(
  CPU& cpu,
  Address to
) -> void
{
  cpu.PC = to;
}

auto BIT(
  CPU& cpu,
  Byte val
) -> void
{
  detail::set_flag_Z(cpu, val & cpu.AC);
  set_flag(cpu.SR, Flag::V, static_cast<bool>(val & (1 << 6)));
  set_flag(cpu.SR, Flag::N, static_cast<bool>(val & (1 << 7)));
}

}  // namespace mos6502
