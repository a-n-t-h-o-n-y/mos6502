#ifndef MOS6502_OPCODES_HPP
#define MOS6502_OPCODES_HPP
#include <algorithm>
#include <array>
#include <stdexcept>

#include <mos6502/addressing_modes.hpp>
#include <mos6502/byte.hpp>
#include <mos6502/cpu.hpp>
#include <mos6502/instructions.hpp>
#include <mos6502/memory.hpp>

// TODO Add Illegal Opcodes

namespace mos6502 {

template <Memory M>
using OpcodeFn = int(*)(CPU&, M&);

template <Memory M>
using OpTable = std::array<OpcodeFn<M>, 256>;

/// Create an opcode indexed table of instructions for a specific Memory type.
/** Clients should create their own opcode table, then use with execute fns. */
template <Memory M>
[[nodiscard]] constexpr
auto create_opcode_table() -> OpTable<M>
{
  auto t = std::array<OpcodeFn<M>, 256>{};
  std::ranges::fill(t, [](CPU&, M&) -> int {
    throw std::invalid_argument{"Illegal Instruction."};
  });

  // ADC
  t[0x69] = [](CPU& c, M& m) {
    ADC(c, IMM(c, m));
    return 2;
  };
  t[0x65] = [](CPU& c, M& m) {
    ADC(c, m.read(ZP0(c, m)));
    return 3;
  };
  t[0x75] = [](CPU& c, M& m) {
    ADC(c, m.read(ZPX(c, m)));
    return 4;
  };
  t[0x6D] = [](CPU& c, M& m) {
    ADC(c, m.read(ABS(c, m)));
    return 4;
  };
  t[0x7D] = [](CPU& c, M& m) {
    auto const [extra, addr] = ABX(c, m);
    ADC(c, m.read(addr));
    return 4 + (extra ? 1 : 0);
  };
  t[0x79] = [](CPU& c, M& m) {
    auto const [extra, addr] = ABY(c, m);
    ADC(c, m.read(addr));
    return 4 + (extra ? 1 : 0);
  };
  t[0x61] = [](CPU& c, M& m) {
    ADC(c, m.read(IZX(c, m)));
    return 6;
  };
  t[0x71] = [](CPU& c, M& m) {
    auto const [extra, addr] = IZY(c, m);
    ADC(c, m.read(addr));
    return 5 + (extra ? 1 : 0);
  };

  // SBC
  t[0xE9] = [](CPU& c, M& m) {
    SBC(c, m.read(IMM(c, m)));
    return 2;
  };
  t[0xE5] = [](CPU& c, M& m) {
    SBC(c, m.read(ZP0(c, m)));
    return 3;
  };
  t[0xF5] = [](CPU& c, M& m) {
    SBC(c, m.read(ZPX(c, m)));
    return 4;
  };
  t[0xED] = [](CPU& c, M& m) {
    SBC(c, m.read(ABS(c, m)));
    return 4;
  };
  t[0xFD] = [](CPU& c, M& m) {
    auto const [extra, addr] = ABX(c, m);
    SBC(c, m.read(addr));
    return 4 + (extra ? 1 : 0);
  };
  t[0xF9] = [](CPU& c, M& m) {
    auto const [extra, addr] = ABY(c, m);
    SBC(c, m.read(addr));
    return 4 + (extra ? 1 : 0);
  };
  t[0xE1] = [](CPU& c, M& m) {
    SBC(c, m.read(IZX(c, m)));
    return 6;
  };
  t[0xF1] = [](CPU& c, M& m) {
    auto const [extra, addr] = IZY(c, m);
    SBC(c, m.read(addr));
    return 5 + (extra ? 1 : 0);
  };

  // LDA
  t[0xA9] = [](CPU& c, M& m) {
    LDA(c, m.read(IMM(c, m)));
    return 2;
  };
  t[0xA5] = [](CPU& c, M& m) {
    LDA(c, m.read(ZP0(c, m)));
    return 3;
  };
  t[0xB5] = [](CPU& c, M& m) {
    LDA(c, m.read(ZPX(c, m)));
    return 4;
  };
  t[0xAD] = [](CPU& c, M& m) {
    LDA(c, m.read(ABS(c, m)));
    return 4;
  };
  t[0xBD] = [](CPU& c, M& m) {
    auto const [extra, addr] = ABX(c, m);
    LDA(c, m.read(addr));
    return 4 + (extra ? 1 : 0);
  };
  t[0xB9] = [](CPU& c, M& m) {
    auto const [extra, addr] = ABY(c, m);
    LDA(c, m.read(addr));
    return 4 + (extra ? 1 : 0);
  };
  t[0xA1] = [](CPU& c, M& m) {
    LDA(c, m.read(IZX(c, m)));
    return 6;
  };
  t[0xB1] = [](CPU& c, M& m) {
    auto const [extra, addr] = ABX(c, m);
    LDA(c, m.read(addr));
    return 5 + (extra ? 1 : 0);
  };

  // LDX
  t[0xA2] = [](CPU& c, M& m) {
    LDX(c, m.read(IMM(c, m)));
    return 2;
  };
  t[0xA6] = [](CPU& c, M& m) {
    LDX(c, m.read(ZP0(c, m)));
    return 3;
  };
  t[0xB6] = [](CPU& c, M& m) {
    LDX(c, m.read(ZPY(c, m)));
    return 4;
  };
  t[0xAE] = [](CPU& c, M& m) {
    LDX(c, m.read(ABS(c, m)));
    return 4;
  };
  t[0xBE] = [](CPU& c, M& m) {
    auto const [extra, addr] = ABY(c, m);
    LDX(c, m.read(addr));
    return 4 + (extra ? 1 : 0);
  };

  // LDY
  t[0xA0] = [](CPU& c, M& m) {
    LDY(c, m.read(IMM(c, m)));
    return 2;
  };
  t[0xA4] = [](CPU& c, M& m) {
    LDY(c, m.read(ZP0(c, m)));
    return 3;
  };
  t[0xB4] = [](CPU& c, M& m) {
    LDY(c, m.read(ZPX(c, m)));
    return 4;
  };
  t[0xAC] = [](CPU& c, M& m) {
    LDY(c, m.read(ABS(c, m)));
    return 4;
  };
  t[0xBC] = [](CPU& c, M& m) {
    auto const [extra, addr] = ABX(c, m);
    LDY(c, m.read(addr));
    return 4 + (extra ? 1 : 0);
  };

  // STA
  t[0x85] = [](CPU& c, M& m) {
    STA(c, m, ZP0(c, m));
    return 3;
  };
  t[0x95] = [](CPU& c, M& m) {
    STA(c, m, ZPX(c, m));
    return 4;
  };
  t[0x8D] = [](CPU& c, M& m) {
    STA(c, m, ABS(c, m));
    return 4;
  };
  t[0x9D] = [](CPU& c, M& m) {
    STA(c, m, ABX(c, m).second);
    return 5;
  };
  t[0x99] = [](CPU& c, M& m) {
    STA(c, m, ABY(c, m).second);
    return 5;
  };
  t[0x81] = [](CPU& c, M& m) {
    STA(c, m, IZX(c, m));
    return 6;
  };
  t[0x91] = [](CPU& c, M& m) {
    STA(c, m, IZY(c, m).second);
    return 6;
  };

  // STX
  t[0x86] = [](CPU& c, M& m) {
    STX(c, m, ZP0(c, m));
    return 3;
  };
  t[0x96] = [](CPU& c, M& m) {
    STX(c, m, ZPY(c, m));
    return 4;
  };
  t[0x8E] = [](CPU& c, M& m) {
    STX(c, m, ABS(c, m));
    return 4;
  };

  // STY
  t[0x84] = [](CPU& c, M& m) {
    STY(c, m, ZP0(c, m));
    return 3;
  };
  t[0x94] = [](CPU& c, M& m) {
    STY(c, m, ZPX(c, m));
    return 4;
  };
  t[0x8C] = [](CPU& c, M& m) {
    STY(c, m, ABS(c, m));
    return 4;
  };

  // TAX
  t[0xAA] = [](CPU& c, M&) {
    IMP();
    TAX(c);
    return 2;
  };

  // TAY
  t[0xA8] = [](CPU& c, M&) {
    IMP();
    TAY(c);
    return 2;
  };

  // TSX
  t[0xBA] = [](CPU& c, M&) {
    IMP();
    TSX(c);
    return 2;
  };

  // TXA
  t[0x8A] = [](CPU& c, M&) {
    IMP();
    TXA(c);
    return 2;
  };

  // TXS
  t[0x9A] = [](CPU& c, M&) {
    IMP();
    TXS(c);
    return 2;
  };

  // TYA
  t[0x98] = [](CPU& c, M&) {
    IMP();
    TYA(c);
    return 2;
  };

  // PHA
  t[0x48] = [](CPU& c, M& m) {
    IMP();
    PHA(c, m);
    return 3;
  };

  // PHP
  t[0x08] = [](CPU& c, M& m) {
    IMP();
    PHP(c, m);
    return 3;
  };

  // PLA
  t[0x68] = [](CPU& c, M& m)
  {
    IMP();
    PLA(c, m);
    return 4;
  };

  // PLP
  t[0x28] = [](CPU& c, M& m)
  {
    IMP();
    PLP(c, m);
    return 4;
  };

  // DEC
  t[0xC6] = [](CPU& c, M& m) {
    DEC(c, m, ZP0(c, m));
    return 5;
  };
  t[0xD6] = [](CPU& c, M& m) {
    DEC(c, m, ZPX(c, m));
    return 6;
  };
  t[0xCE] = [](CPU& c, M& m) {
    DEC(c, m, ABS(c, m));
    return 6;
  };
  t[0xDE] = [](CPU& c, M& m) {
    DEC(c, m, ABX(c, m).second);
    return 7;
  };

  // DEX
  t[0xCA] = [](CPU& c, M&) {
    IMP();
    DEX(c);
    return 2;
  };

  // DEY
  t[0x88] = [](CPU& c, M&) {
    IMP();
    DEY(c);
    return 2;
  };

  // INC
  t[0xE6] = [](CPU& c, M& m) {
    INC(c, m, ZP0(c, m));
    return 5;
  };
  t[0xF6] = [](CPU& c, M& m) {
    INC(c, m, ZPX(c, m));
    return 6;
  };
  t[0xEE] = [](CPU& c, M& m) {
    INC(c, m, ABS(c, m));
    return 6;
  };
  t[0xFE] = [](CPU& c, M& m) {
    INC(c, m, ABX(c, m).second);
    return 7;
  };

  // INX
  t[0xE8] = [](CPU& c, M&) {
    IMP();
    INX(c);
    return 2;
  };

  // INY
  t[0xC8] = [](CPU& c, M&) {
    IMP();
    INY(c);
    return 2;
  };

  // AND
  t[0x29] = [](CPU& c, M& m) {
    AND(c, IMM(c, m));
    return 2;
  };
  t[0x25] = [](CPU& c, M& m) {
    AND(c, m.read(ZP0(c, m)));
    return 3;
  };
  t[0x35] = [](CPU& c, M& m) {
    AND(c, m.read(ZPX(c, m)));
    return 4;
  };
  t[0x2D] = [](CPU& c, M& m) {
    AND(c, m.read(ABS(c, m)));
    return 4;
  };
  t[0x3D] = [](CPU& c, M& m) {
    auto const [extra, addr] = ABX(c, m);
    AND(c, m.read(addr));
    return 4 + (extra ? 1 : 0);
  };
  t[0x39] = [](CPU& c, M& m) {
    auto const [extra, addr] = ABY(c, m);
    AND(c, m.read(addr));
    return 4 + (extra ? 1 : 0);
  };
  t[0x21] = [](CPU& c, M& m) {
    AND(c, m.read(IZX(c, m)));
    return 6;
  };
  t[0x31] = [](CPU& c, M& m) {
    auto const [extra, addr] = IZY(c, m);
    AND(c, m.read(addr));
    return 5 + (extra ? 1 : 0);
  };

  // EOR
  t[0x49] = [](CPU& c, M& m) {
    EOR(c, IMM(c, m));
    return 2;
  };
  t[0x45] = [](CPU& c, M& m) {
    EOR(c, m.read(ZP0(c, m)));
    return 3;
  };
  t[0x55] = [](CPU& c, M& m) {
    EOR(c, m.read(ZPX(c, m)));
    return 4;
  };
  t[0x4D] = [](CPU& c, M& m) {
    EOR(c, m.read(ABS(c, m)));
    return 4;
  };
  t[0x5D] = [](CPU& c, M& m) {
    auto const [extra, addr] = ABX(c, m);
    EOR(c, m.read(addr));
    return 4 + (extra ? 1 : 0);
  };
  t[0x59] = [](CPU& c, M& m) {
    auto const [extra, addr] = ABY(c, m);
    EOR(c, m.read(addr));
    return 4 + (extra ? 1 : 0);
  };
  t[0x41] = [](CPU& c, M& m) {
    EOR(c, m.read(IZX(c, m)));
    return 6;
  };
  t[0x51] = [](CPU& c, M& m) {
    auto const [extra, addr] = IZY(c, m);
    EOR(c, m.read(addr));
    return 5 + (extra ? 1 : 0);
  };


  // ORA
  t[0x09] = [](CPU& c, M& m) {
    ORA(c, IMM(c, m));
    return 2;
  };
  t[0x05] = [](CPU& c, M& m) {
    ORA(c, m.read(ZP0(c, m)));
    return 3;
  };
  t[0x15] = [](CPU& c, M& m) {
    ORA(c, m.read(ZPX(c, m)));
    return 4;
  };
  t[0x0D] = [](CPU& c, M& m) {
    ORA(c, m.read(ABS(c, m)));
    return 4;
  };
  t[0x1D] = [](CPU& c, M& m) {
    auto const [extra, addr] = ABX(c, m);
    ORA(c, m.read(addr));
    return 4 + (extra ? 1 : 0);
  };
  t[0x19] = [](CPU& c, M& m) {
    auto const [extra, addr] = ABY(c, m);
    ORA(c, m.read(addr));
    return 4 + (extra ? 1 : 0);
  };
  t[0x01] = [](CPU& c, M& m) {
    ORA(c, m.read(IZX(c, m)));
    return 6;
  };
  t[0x11] = [](CPU& c, M& m) {
    auto const [extra, addr] = IZY(c, m);
    ORA(c, m.read(addr));
    return 5 + (extra ? 1 : 0);
  };

  // ASL
  t[0x0A] = [](CPU& c, M&) {
    ASL_ACC(c, ACC(c));
    return 2;
  };
  t[0x06] = [](CPU& c, M& m) {
    ASL_MEM(c, m, ZP0(c, m));
    return 5;
  };
  t[0x16] = [](CPU& c, M& m) {
    ASL_MEM(c, m, ZPX(c, m));
    return 6;
  };
  t[0x0E] = [](CPU& c, M& m) {
    ASL_MEM(c, m, ABS(c, m));
    return 6;
  };
  t[0x1E] = [](CPU& c, M& m) {
    ASL_MEM(c, m, ABX(c, m).second);
    return 7;
  };

  // LSR
  t[0x4A] = [](CPU& c, M&) {
    LSR_ACC(c, ACC(c));
    return 2;
  };
  t[0x46] = [](CPU& c, M& m) {
    LSR_MEM(c, m, ZP0(c, m));
    return 5;
  };
  t[0x56] = [](CPU& c, M& m) {
    LSR_MEM(c, m, ZPX(c, m));
    return 6;
  };
  t[0x4E] = [](CPU& c, M& m) {
    LSR_MEM(c, m, ABS(c, m));
    return 6;
  };
  t[0x5E] = [](CPU& c, M& m) {
    LSR_MEM(c, m, ABX(c, m).second);
    return 7;
  };

  // ROL
  t[0x2A] = [](CPU& c, M&) {
    ROL_ACC(c, ACC(c));
    return 2;
  };
  t[0x26] = [](CPU& c, M& m) {
    ROL_MEM(c, m, ZP0(c, m));
    return 5;
  };
  t[0x36] = [](CPU& c, M& m) {
    ROL_MEM(c, m, ZPX(c, m));
    return 6;
  };
  t[0x2E] = [](CPU& c, M& m) {
    ROL_MEM(c, m, ABS(c, m));
    return 6;
  };
  t[0x3E] = [](CPU& c, M& m) {
    ROL_MEM(c, m, ABX(c, m).second);
    return 7;
  };

  // ROR
  t[0x6A] = [](CPU& c, M&) {
    ROR_ACC(c, ACC(c));
    return 2;
  };
  t[0x66] = [](CPU& c, M& m) {
    ROR_MEM(c, m, ZP0(c, m));
    return 5;
  };
  t[0x76] = [](CPU& c, M& m) {
    ROR_MEM(c, m, ZPX(c, m));
    return 6;
  };
  t[0x6E] = [](CPU& c, M& m) {
    ROR_MEM(c, m, ABS(c, m));
    return 6;
  };
  t[0x7E] = [](CPU& c, M& m) {
    ROR_MEM(c, m, ABX(c, m).second);
    return 7;
  };

  // CLC
  t[0x18] = [](CPU& c, M&) {
    IMP();
    CLC(c);
    return 2;
  };

  // CLD
  t[0xD8] = [](CPU& c, M&) {
    IMP();
    CLD(c);
    return 2;
  };

  // CLI
  t[0x58] = [](CPU& c, M&) {
    IMP();
    CLI(c);
    return 2;
  };

  // CLV
  t[0xB8] = [](CPU& c, M&) {
    IMP();
    CLV(c);
    return 2;
  };

  // SEC
  t[0x38] = [](CPU& c, M&) {
    IMP();
    SEC(c);
    return 2;
  };

  // SED
  t[0xF8] = [](CPU& c, M&) {
    IMP();
    SED(c);
    return 2;
  };

  // SEI
  t[0x78] = [](CPU& c, M&) {
    IMP();
    SEI(c);
    return 2;
  };

  // CMP
  t[0xC9] = [](CPU& c, M& m) {
    CMP(c, IMM(c, m));
    return 2;
  };
  t[0xC5] = [](CPU& c, M& m) {
    CMP(c, m.read(ZP0(c, m)));
    return 3;
  };
  t[0xD5] = [](CPU& c, M& m) {
    CMP(c, m.read(ZPX(c, m)));
    return 4;
  };
  t[0xCD] = [](CPU& c, M& m) {
    CMP(c, m.read(ABS(c, m)));
    return 4;
  };
  t[0xDD] = [](CPU& c, M& m) {
    auto const [extra, addr] = ABX(c, m);
    CMP(c, m.read(addr));
    return 4 + (extra ? 1 : 0);
  };
  t[0xD9] = [](CPU& c, M& m) {
    auto const [extra, addr] = ABY(c, m);
    CMP(c, m.read(addr));
    return 4 + (extra ? 1 : 0);
  };
  t[0xC1] = [](CPU& c, M& m) {
    CMP(c, m.read(IZX(c, m)));
    return 6;
  };
  t[0xD1] = [](CPU& c, M& m) {
    auto const [extra, addr] = IZY(c, m);
    CMP(c, m.read(addr));
    return 5 + (extra ? 1 : 0);
  };

  // CPX
  t[0xE0] = [](CPU& c, M& m) {
    CPX(c, IMM(c, m));
    return 2;
  };
  t[0xE4] = [](CPU& c, M& m) {
    CPX(c, m.read(ZP0(c, m)));
    return 3;
  };
  t[0xEC] = [](CPU& c, M& m) {
    CPX(c, m.read(ABS(c, m)));
    return 4;
  };

  // CPY
  t[0xC0] = [](CPU& c, M& m) {
    CPY(c, IMM(c, m));
    return 2;
  };
  t[0xC4] = [](CPU& c, M& m) {
    CPY(c, m.read(ZP0(c, m)));
    return 3;
  };
  t[0xCC] = [](CPU& c, M& m) {
    CPY(c, m.read(ABS(c, m)));
    return 4;
  };


  // BCC
  t[0x90] = [](CPU& c, M& m) {
    return 2 + BCC(c, REL(c, m));
  };

  // BCS
  t[0xB0] = [](CPU& c, M& m) {
    return 2 + BCS(c, REL(c, m));
  };

  // BEQ
  t[0xF0] = [](CPU& c, M& m) {
    return 2 + BEQ(c, REL(c, m));
  };

  // BMI
  t[0x30] = [](CPU& c, M& m) {
    return 2 + BMI(c, REL(c, m));
  };

  // BNE
  t[0xD0] = [](CPU& c, M& m) {
    return 2 + BNE(c, REL(c, m));
  };

  // BPL
  t[0x10] = [](CPU& c, M& m) {
    return 2 + BPL(c, REL(c, m));
  };

  // BVC
  t[0x50] = [](CPU& c, M& m) {
    return 2 + BVC(c, REL(c, m));
  };

  // BVS
  t[0x70] = [](CPU& c, M& m) {
    return 2 + BVS(c, REL(c, m));
  };

  // JMP
  t[0x4C] = [](CPU& c, M& m) {
    JMP(c, ABS(c, m));
    return 3;
  };
  t[0x6C] = [](CPU& c, M& m) {
    JMP(c, IND(c, m));
    return 5;
  };

  // JSR
  t[0x20] = [](CPU& c, M& m) {
    JSR(c, m, ABS(c, m));
    return 6;
  };

  // RTS
  t[0x60] = [](CPU& c, M& m) {
    IMP();
    RTS(c, m);
    return 6;
  };

  // BRK
  t[0x00] = [](CPU& c, M& m) {
    IMP();
    BRK(c, m);
    return 7;
  };

  // RTI
  t[0x40] = [](CPU& c, M& m) {
    IMP();
    RTI(c, m);
    return 6;
  };

  // BIT
  t[0x24] = [](CPU& c, M& m) {
    BIT(c, m.read(ZP0(c, m)));
    return 3;
  };
  t[0x2C] = [](CPU& c, M& m) {
    BIT(c, m.read(ABS(c, m)));
    return 4;
  };

  // NOP
  t[0xEA] = [](CPU&, M&) {
    IMP();
    NOP();
    return 2;
  };

  return t;
}

}  // namespace mos6502
#endif  // MOS6502_OPCODES_HPP
