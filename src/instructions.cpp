#include <mos6502/instructions.hpp>

#include <mos6502/word.hpp>

namespace {

using namespace mos6502;

/// Set the Negative Flag
auto set_flag_N(
  CPU& cpu,
  Byte value
) -> void
{
  set_flag(
    cpu.SR,
    Flag::N,
    static_cast<bool>(value & 0x80)
  );
}

/// Set the Zero Flag
auto set_flag_Z(
  CPU& cpu,
  Byte value
) -> void
{
  set_flag(
    cpu.SR,
    Flag::Z,
    value == 0
  );
}

/// Set the Carry Flag
auto set_flag_C(
  CPU& cpu,
  Word value
) -> void
{
  set_flag(
    cpu.SR,
    Flag::C,
    static_cast<bool>(value & (0x0001 << 8))
  );
}

/// Pull single Byte from stack pointer location and increment stack pointer.
[[nodiscard]]
auto stack_pull(
  CPU& cpu,
  Memory const& mem
) -> Byte
{
  cpu.SP += 1;
  return mem.read(0x0100 + cpu.SP);
}

/// Push single Byte to the current stack pointer, then decrement stack pointer.
auto stack_push(
  CPU& cpu,
  Memory& mem,
  Byte value
) -> void
{
  mem.write(0x0100 + cpu.SP, value);
  cpu.SP -= 1;
}

/// Shift Left One Bit. Carry is set to MSB before shift.
auto ASL(
  CPU& cpu,
  Byte val
) -> Byte
{
  // Set carry flag before MSB is shifted out of existence.
  set_flag(
    cpu.SR,
    Flag::C,
    val & (0x01 << 7)
  );
  Byte const r = val << 1;
  set_flag_N(cpu, r);
  set_flag_Z(cpu, r);
  return r;
}

/// Shift Right One Bit. Carry is set to LSB before shift.
auto LSR(
  CPU& cpu,
  Byte val
) -> Byte
{
  set_flag(
    cpu.SR,
    Flag::C,
    val & 0x01
  );
  Byte const r = val >> 1;
  set_flag_N(cpu, r);
  set_flag_Z(cpu, r);
  return r;
}

/// Rotate Left One Bit. MSB set as carry, original carry placed in LSB.
auto ROL(
  CPU& cpu,
  Byte val
) -> Byte
{
  // Important to get the carry flag before ASL() overwrites it.
  Byte const c = get_flag(cpu.SR, Flag::C);
  Byte const r = ASL(cpu, val) | c;
  set_flag_Z(cpu, r);
  return r;
}

/// Rotate Right One Bit. LSB set as carry, original carry placed in MSB.
auto ROR(
  CPU& cpu,
  Byte val
) -> Byte
{
  // Important to get the carry flag before LSR() overwrites it.
  Byte const c = get_flag(cpu.SR, Flag::C);
  Byte const r = LSR(cpu, val) | (c << 7);
  set_flag_N(cpu, r);
  return r;
}

/// Updates CPU Status Flags based on result of reg - val;
auto compare(
  CPU& cpu,
  Byte reg,
  Byte val
) -> void
{
  Byte const result = reg - val;
  set_flag_Z(cpu, result);
  set_flag_N(cpu, result);
  set_flag(cpu.SR, Flag::C, reg >= val);
}

/**
 * @brief Branch if condition is met.
 * @param condition - the condition to check
 * @param cpu - reference to the CPU object
 * @param address - the address to jump to if condition is true
 * @return int - number of extra cycles used (in range [0-2])
 */
[[nodiscard]]
auto branch_if(
  bool condition,
  CPU& cpu,
  Address address
) -> int
{
  if (condition) {
    bool const page_change = (address & 0xFF00) != (cpu.PC & 0xFF00);
    cpu.PC = address;
    return page_change ? 2 : 1;
  }
  return 0;
}

/// Reads two bytes in lo byte, hi byte order and returns as Address.
[[nodiscard]]
auto read_address(
  Memory const& mem,
  Address at
) -> Address
{
  return Address{mem.read(at)} | (Address{mem.read(at + 1)} << 8);
}

/// Handles interrupts, pushes PC and SR to stack, sets flags, jumps to Address.
auto interrupt(
  CPU& cpu,
  Memory& mem,
  Address to,
  bool is_software
) -> void
{
  // PC
  Byte const hi = Byte((cpu.PC >> 8) & 0x00FF);
  Byte const lo = Byte(cpu.PC & 0x00FF);
  stack_push(cpu, mem, hi);
  stack_push(cpu, mem, lo);

  // Status Register
  Byte status = cpu.SR;
  set_flag(status, Flag::B, is_software);
  set_flag(status, Flag::U, true);
  stack_push(cpu, mem, status);

  // Set Interrupt Flag
  set_flag(cpu.SR, Flag::I, true);

  // Jump
  cpu.PC = read_address(mem, to);
}

[[nodiscard]]
auto pull_PC(
  CPU& cpu,
  Memory const& mem
) -> Address
{
  Byte const lo = stack_pull(cpu, mem);
  Byte const hi = stack_pull(cpu, mem);
  return (hi << 8) | lo;
}

}  // namespace

namespace nes {

auto ADC(
  CPU& cpu,
  Byte value
) -> void
{
  Word const r = static_cast<Word>(
    cpu.AC + value + static_cast<Word>(get_flag(cpu.SR, Flag::C))
  );

  // Carry Flag
  set_flag_C(cpu, r);

  // Zero Flag
  set_flag_Z(cpu, static_cast<Byte>(r));

  // Signed Overflow Flag
  set_flag(
    cpu.SR,
    Flag::V,
    static_cast<bool>((
      ~(static_cast<Word>(cpu.AC) ^ static_cast<Word>(value)) &
       (static_cast<Word>(cpu.AC) ^ static_cast<Word>(r))
    ) & 0x0080)
  );

  // Negative Flag
  set_flag_N(cpu, static_cast<Byte>(r));

  cpu.AC = static_cast<Byte>(r);
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

auto LDA(
  CPU& cpu,
  Byte value
) -> void
{
  cpu.AC = value;
  set_flag_N(cpu, value);
  set_flag_Z(cpu, value);
}

auto LDX(
  CPU& cpu,
  Byte value
) -> void
{
  cpu.X = value;
  set_flag_N(cpu, value);
  set_flag_Z(cpu, value);
}

auto LDY(
  CPU& cpu,
  Byte value
) -> void
{
  cpu.Y = value;
  set_flag_N(cpu, value);
  set_flag_Z(cpu, value);
}

auto STA(
  CPU& cpu,
  Memory& mem,
  Address at
) -> void
{
  mem.write(at, cpu.AC);
}

auto STX(
  CPU& cpu,
  Memory& mem,
  Address at
) -> void
{
  mem.write(at, cpu.X);
}

auto STY(
  CPU& cpu,
  Memory& mem,
  Address at
) -> void
{
  mem.write(at, cpu.Y);
}

auto TAX(
  CPU& cpu
) -> void
{
  cpu.X = cpu.AC;
  set_flag_N(cpu, cpu.X);
  set_flag_Z(cpu, cpu.X);
}

auto TAY(
  CPU& cpu
) -> void
{
  cpu.Y = cpu.AC;
  set_flag_N(cpu, cpu.Y);
  set_flag_Z(cpu, cpu.Y);
}

auto TSX(
  CPU& cpu
) -> void
{
  cpu.X = cpu.SP;
  set_flag_N(cpu, cpu.X);
  set_flag_Z(cpu, cpu.X);
}

auto TXA(
  CPU& cpu
) -> void
{
  cpu.AC = cpu.X;
  set_flag_N(cpu, cpu.AC);
  set_flag_Z(cpu, cpu.AC);
}

auto TXS(
  CPU& cpu
) -> void
{
  cpu.SP = cpu.X;
  set_flag_N(cpu, cpu.SP);
  set_flag_Z(cpu, cpu.SP);
}

auto TYA(
  CPU& cpu
) -> void
{
  cpu.AC = cpu.Y;
  set_flag_N(cpu, cpu.AC);
  set_flag_Z(cpu, cpu.AC);
}

auto PHA(
  CPU& cpu,
  Memory& mem
) -> void
{
  stack_push(cpu, mem, cpu.AC);
}

auto PHP(
  CPU& cpu,
  Memory& mem
) -> void
{
  Byte status = cpu.SR;
  set_flag(status, Flag::B, true);
  set_flag(status, Flag::U, true);
  stack_push(cpu, mem, status);
}

auto PLA(
  CPU& cpu,
  Memory const& mem
) -> void
{
  cpu.AC = stack_pull(cpu, mem);
  set_flag_N(cpu, cpu.AC);
  set_flag_Z(cpu, cpu.AC);
}

auto PLP(
  CPU& cpu,
  Memory const& mem
) -> void
{
  cpu.SR = stack_pull(cpu, mem);
  set_flag(cpu.SR, Flag::B, false);
  set_flag(cpu.SR, Flag::U, false);
}

auto DEC(
  CPU& cpu,
  Memory& mem,
  Address at
) -> void
{
  Byte const next = mem.read(at) - 1;
  mem.write(at, next);
  set_flag_N(cpu, next);
  set_flag_Z(cpu, next);
}

auto DEX(
  CPU& cpu
) -> void
{
  cpu.X -= 1;
  set_flag_N(cpu, cpu.X);
  set_flag_Z(cpu, cpu.X);
}

auto DEY(
  CPU& cpu
) -> void
{
  cpu.Y -= 1;
  set_flag_N(cpu, cpu.Y);
  set_flag_Z(cpu, cpu.Y);
}

auto INC(
  CPU& cpu,
  Memory& mem,
  Address at
) -> void
{
  Byte const next = mem.read(at) + 1;
  mem.write(at, next);
  set_flag_N(cpu, next);
  set_flag_Z(cpu, next);
}

auto INX(
  CPU& cpu
) -> void
{
  cpu.X += 1;
  set_flag_N(cpu, cpu.X);
  set_flag_Z(cpu, cpu.X);
}

auto INY(
  CPU& cpu
) -> void
{
  cpu.Y += 1;
  set_flag_N(cpu, cpu.Y);
  set_flag_Z(cpu, cpu.Y);
}

auto AND(
  CPU& cpu,
  Byte value
) -> void
{
  cpu.AC &= value;
  set_flag_N(cpu, cpu.AC);
  set_flag_Z(cpu, cpu.AC);
}

auto EOR(
  CPU& cpu,
  Byte value
) -> void
{
  cpu.AC ^= value;
  set_flag_N(cpu, cpu.AC);
  set_flag_Z(cpu, cpu.AC);
}

auto ORA(
  CPU& cpu,
  Byte value
) -> void
{
  cpu.AC |= value;
  set_flag_N(cpu, cpu.AC);
  set_flag_Z(cpu, cpu.AC);
}

auto ASL_ACC(
  CPU& cpu,
  Byte val
) -> void
{
  cpu.AC = ASL(cpu, val);
}

auto ASL_MEM(
  CPU& cpu,
  Memory& mem,
  Address at
) -> void
{
  mem.write(at, ASL(cpu, mem.read(at)));
}

auto LSR_ACC(
  CPU& cpu,
  Byte val
) -> void
{
  cpu.AC = LSR(cpu, val);
}

auto LSR_MEM(
  CPU& cpu,
  Memory& mem,
  Address at
) -> void
{
  mem.write(at, LSR(cpu, mem.read(at)));
}

auto ROL_ACC(
  CPU& cpu,
  Byte val
) -> void
{
  cpu.AC = ROL(cpu, val);
}

auto ROL_MEM(
  CPU& cpu,
  Memory& mem,
  Address at
) -> void
{
  mem.write(at, ROL(cpu, mem.read(at)));
}

auto ROR_ACC(
  CPU& cpu,
  Byte val
) -> void
{
  cpu.AC = ROR(cpu, val);
}

auto ROR_MEM(
  CPU& cpu,
  Memory& mem,
  Address at
) -> void
{
  mem.write(at, ROR(cpu, mem.read(at)));
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
  compare(cpu, cpu.AC, val);
}

auto CPX(
  CPU& cpu,
  Byte val
) -> void
{
  compare(cpu, cpu.X, val);
}

auto CPY(
  CPU& cpu,
  Byte val
) -> void
{
  compare(cpu, cpu.Y, val);
}

auto BCC(
  CPU& cpu,
  Address to
) -> int
{
  return branch_if(
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
  return branch_if(
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
  return branch_if(
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
  return branch_if(
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
  return branch_if(
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
  return branch_if(
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
  return branch_if(
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
  return branch_if(
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

auto JSR(
  CPU& cpu,
  Memory& mem,
  Address to
) -> void
{
  stack_push(cpu, mem, static_cast<Byte>((cpu.PC >> 8) & 0x00FF));
  stack_push(cpu, mem, static_cast<Byte>(cpu.PC & 0x00FF));
  JMP(cpu, to);
}

auto RTS(
  CPU& cpu,
  Memory const& mem
) -> void
{
  cpu.PC = pull_PC(cpu, mem);
}

auto BRK(
  CPU& cpu,
  Memory& mem
) -> void
{
  cpu.PC += 1;  // Skip over operand byte.
  interrupt(cpu, mem, 0xFFFE, true);
}

auto RTI(
  CPU& cpu,
  Memory const& mem
) -> void
{
  // Pull SR
  PLP(cpu, mem);

  // Pull PC
  RTS(cpu, mem);
}

auto BIT(
  CPU& cpu,
  Byte val
) -> void
{
  set_flag_Z(cpu, val & cpu.AC);
  set_flag(cpu.SR, Flag::V, static_cast<bool>(val & (1 << 6)));
  set_flag(cpu.SR, Flag::N, static_cast<bool>(val & (1 << 7)));
}

auto NOP() -> void {}

auto IRQ(
  CPU& cpu,
  Memory& mem
) -> int
{
  if (get_flag(cpu.SR, Flag::I)) {
    return 0;
  } else {
    interrupt(cpu, mem, 0xFFFE, false);
    return 7;
  }
}

auto NMI(
  CPU& cpu,
  Memory& mem
) -> int
{
  interrupt(cpu, mem, 0xFFFA, false);
  return 8;
}

auto reset(
  CPU& cpu,
  Memory& mem
) -> int
{
  cpu = CPU{};
  cpu.PC = read_address(mem, 0xFFFC);
  return 8;
}

}  // namespace nes
