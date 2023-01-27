#ifndef MOS6502_INSTRUCTIONS_HPP
#define MOS6502_INSTRUCTIONS_HPP
#include <mos6502/byte.hpp>
#include <mos6502/cpu.hpp>
#include <mos6502/detail/instruction_helpers.hpp>
#include <mos6502/memory.hpp>

namespace mos6502 {

// Arithmetic - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/// Add Memory to Accumulator with Carry.
/** Always has the potential to add another cycle. */
auto ADC(
  CPU&,
  Byte
) -> void;

/// Subtract Memory from Accumulator with Borrow.
/** Always has the potential to add another cycle. */
auto SBC(
  CPU&,
  Byte
) -> void;


// Load, Store, Transfer - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/// Load Accumulator with Memory
auto LDA(
  CPU&,
  Byte
) -> void;

/// Load Register X with Memory
auto LDX(
  CPU&,
  Byte
) -> void;

/// Load Register Y with Memory
auto LDY(
  CPU&,
  Byte
) -> void;

/// Store Accumulator in Memory
auto STA(
  CPU& cpu,
  Memory auto& mem,
  Address at
) -> void
{
  mem.write(at, cpu.AC);
}

/// Store Register X in Memory
auto STX(
  CPU& cpu,
  Memory auto& mem,
  Address at
) -> void
{
  mem.write(at, cpu.X);
}

/// Store Register Y in Memory
auto STY(
  CPU& cpu,
  Memory auto& mem,
  Address at
) -> void
{
  mem.write(at, cpu.Y);
}

/// Transfer Accumulator to Register X
auto TAX(
  CPU&
) -> void;

/// Transfer Accumulator to Register Y
auto TAY(
  CPU&
) -> void;

/// Transfer Stack Pointer to Register X
auto TSX(
  CPU&
) -> void;

/// Transfer Register X to Accumulator
auto TXA(
  CPU&
) -> void;

/// Transfer Register X to Stack Register
auto TXS(
  CPU&
) -> void;

/// Transfer Register Y to Accumulator
auto TYA(
  CPU&
) -> void;


// Stack Instructions - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/// Push Accumulator on Stack
auto PHA(
  CPU& cpu,
  Memory auto& mem
) -> void
{
  detail::stack_push(cpu, mem, cpu.AC);
}

/// Push Processor Status on Stack
auto PHP(
  CPU& cpu,
  Memory auto& mem
) -> void
{
  Byte status = cpu.SR;
  set_flag(status, Flag::B, true);
  set_flag(status, Flag::U, true);
  detail::stack_push(cpu, mem, status);
}

/// Pull Accumulator from Stack
auto PLA(
  CPU& cpu,
  Memory auto const& mem
) -> void
{
  cpu.AC = detail::stack_pull(cpu, mem);
  detail::set_flag_N(cpu, cpu.AC);
  detail::set_flag_Z(cpu, cpu.AC);
}

/// Pull Processor Status from Stack
auto PLP(
  CPU& cpu,
  Memory auto const& mem
) -> void
{
  cpu.SR = detail::stack_pull(cpu, mem);
  set_flag(cpu.SR, Flag::B, false);
  set_flag(cpu.SR, Flag::U, false);
}


// Decrements + Increments - - - - - - - - - - - - - - - - - - - - - - - - - - -

/// Decrement Memory by One
auto DEC(
  CPU& cpu,
  Memory auto& mem,
  Address at
) -> void
{
  Byte const next = mem.read(at) - 1;
  mem.write(at, next);
  detail::set_flag_N(cpu, next);
  detail::set_flag_Z(cpu, next);
}

/// Decrement Register X by One
auto DEX(
  CPU&
) -> void;

/// Decrement Register Y by One
auto DEY(
  CPU&
) -> void;

/// Increment Memory by One
auto INC(
  CPU& cpu,
  Memory auto& mem,
  Address at
) -> void
{
  Byte const next = mem.read(at) + 1;
  mem.write(at, next);
  detail::set_flag_N(cpu, next);
  detail::set_flag_Z(cpu, next);
}

/// Increment Register X by One
auto INX(
  CPU&
) -> void;

/// Increment Register Y by One
auto INY(
  CPU&
) -> void;


// Logical Operations - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// AND Memory with Accumulator
auto AND(
  CPU&,
  Byte
) -> void;

// Exclusive-OR Memory with Accumulator
auto EOR(
  CPU&,
  Byte
) -> void;

// OR Memory with Accumulator
auto ORA(
  CPU&,
  Byte
) -> void;


// Shift + Rotate - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/// Shift Left One Bit (Accumulator)
auto ASL_ACC(
  CPU&,
  Byte
) -> void;

/// Shift Left One Bit (Memory)
auto ASL_MEM(
  CPU& cpu,
  Memory auto& mem,
  Address at
) -> void
{
  mem.write(at, detail::ASL(cpu, mem.read(at)));
}

/// Shift One Bit Right (Accumulator)
auto LSR_ACC(
  CPU&,
  Byte
) -> void;

/// Shift One Bit Right (Memory)
auto LSR_MEM(
  CPU& cpu,
  Memory auto& mem,
  Address at
) -> void
{
  mem.write(at, detail::LSR(cpu, mem.read(at)));
}

/// Rotate One Bit Left (Accumulator)
auto ROL_ACC(
  CPU&,
  Byte
) -> void;

/// Rotate One Bit Left (Memory)
auto ROL_MEM(
  CPU& cpu,
  Memory auto& mem,
  Address at
) -> void
{
  mem.write(at, detail::ROL(cpu, mem.read(at)));
}

/// Rotate One Bit Right (Accumulator)
auto ROR_ACC(
  CPU&,
  Byte
) -> void;

/// Rotate One Bit Right (Memory)
auto ROR_MEM(
  CPU& cpu,
  Memory auto& mem,
  Address at
) -> void
{
  mem.write(at, detail::ROR(cpu, mem.read(at)));
}


// Flag Instructions - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/// Clear Carry Flag
auto CLC(
  CPU&
) -> void;

/// Clear Decimal Flag
auto CLD(
  CPU&
) -> void;

/// Clear Interrupt Disable Flag
auto CLI(
  CPU&
) -> void;

/// Clear Overflow Flag
auto CLV(
  CPU&
) -> void;

/// Set Carry Flag
auto SEC(
  CPU&
) -> void;

/// Set Decimal Flag
auto SED(
  CPU&
) -> void;

/// Set Interrupt Disable Flag
auto SEI(
  CPU&
) -> void;


// Comparisons

/// Compare Memory with Accumulator
auto CMP(
  CPU&,
  Byte
) -> void;

/// Compare Memory with X Register
auto CPX(
  CPU&,
  Byte
) -> void;

/// Compare Memory with Y Register
auto CPY(
  CPU&,
  Byte
) -> void;


// Conditional Branch Instructions
// Each returns number of extra cycles used, in range [0-2].

/// Branch on Carry Clear.
[[nodiscard]]
auto BCC(
  CPU&,
  Address
) -> int;

/// Branch on Carry Set
[[nodiscard]]
auto BCS(
  CPU&,
  Address
) -> int;

/// Branch on Result Zero
[[nodiscard]]
auto BEQ(
  CPU&,
  Address
) -> int;

/// Branch on Result Minus
[[nodiscard]]
auto BMI(
  CPU&,
  Address
) -> int;

/// Branch on Result not Zero
[[nodiscard]]
auto BNE(
  CPU&,
  Address
) -> int;

/// Branch on Result Plus
[[nodiscard]]
auto BPL(
  CPU&,
  Address
) -> int;

/// Branch on Overflow Clear
[[nodiscard]]
auto BVC(
  CPU&,
  Address
) -> int;

/// Branch on Overflow Set
[[nodiscard]]
auto BVS(
  CPU&,
  Address
) -> int;


// Jumps + Subroutines

/// Jump to New Location
auto JMP(
  CPU&,
  Address
) -> void;

/// Jump to New Location Saving Return Address
auto JSR(
  CPU& cpu,
  Memory auto& mem,
  Address to
) -> void
{
  detail::stack_push(
    cpu,
    mem,
    static_cast<Byte>((cpu.PC >> 8) & 0x00FF)
  );
  detail::stack_push(
    cpu,
    mem,
    static_cast<Byte>(cpu.PC & 0x00FF)
  );
  JMP(cpu, to);
}

/// Return from Subroutine
auto RTS(
  CPU& cpu,
  Memory auto const& mem
) -> void
{
  cpu.PC = detail::pull_PC(cpu, mem);
}


// Interrupts

/// Force Break
auto BRK(
  CPU& cpu,
  Memory auto& mem
) -> void
{
  cpu.PC += 1;  // Skip over operand byte.
  detail::interrupt(cpu, mem, 0xFFFE, true);
}

/// Return from Interrupt
auto RTI(
  CPU& cpu,
  Memory auto const& mem
) -> void
{
  // Pull SR
  PLP(cpu, mem);

  // Pull PC
  RTS(cpu, mem);
}


// Others

/// Test Bits in Memory with Accumulator
auto BIT(
  CPU&,
  Byte
) -> void;

/// No Operation
inline
auto NOP() -> void {}


// Not Actual Instructions

/// Interrupt Request - Maskable - Returns number of cycles used.
auto IRQ(
  CPU& cpu,
  Memory auto& mem
) -> int
{
  if (get_flag(cpu.SR, Flag::I)) {
    return 0;
  } else {
    detail::interrupt(cpu, mem, 0xFFFE, false);
    return 7;
  }
}

/// Interrupt Request - Non-Maskable - Returns number of cycles used.
auto NMI(
  CPU& cpu,
  Memory auto& mem
) -> int
{
  detail::interrupt(cpu, mem, 0xFFFA, false);
  return 8;
}

/// Reset 6502 to a Known State - Returns number of cycles used.
auto reset(
  CPU& cpu,
  Memory auto& mem
) -> int
{
  cpu    = CPU{};
  cpu.PC = detail::read_address(mem, 0xFFFC);
  return 8;
}

}  // namespace mos6502
#endif  // MOS6502_INSTRUCTIONS_HPP
