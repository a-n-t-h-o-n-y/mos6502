#ifndef NES_OPCODES_HPP
#define NES_OPCODES_HPP
#include <algorithm>
#include <array>
#include <stdexcept>

#include <nes/address_modes.hpp>
#include <nes/byte.hpp>
#include <nes/cpu.hpp>
#include <nes/instructions.hpp>
#include <nes/memory.hpp>

namespace nes {

using OpcodeFn = int(*)(CPU&, Memory&);

constexpr auto create_opcode_table() -> std::array<OpcodeFn, 256>
{
  auto t = std::array<OpcodeFn, 256>{};
  std::ranges::fill(t, [](CPU&, Memory&) -> int {
    throw std::invalid_argument{"Illegal Instruction."};
  });

  // ADC
  t[0x69] = [](CPU& c, Memory& m)
  {
    ADC(c, IMM(c, m));
    return 2;
  };
  t[0x65] = [](CPU& c, Memory& m) {
    ADC(c, m.read(ZP0(c, m)));
    return 3;
  };
  t[0x75] = [](CPU& c, Memory& m)
  {
    ADC(c, m.read(ZPX(c, m)));
    return 4;
  };
  t[0x6D] = [](CPU& c, Memory& m)
  {
    ADC(c, m.read(ABS(c, m)));
    return 4;
  };
  t[0x7D] = [](CPU& c, Memory& m)
  {
    auto const [extra, addr] = ABX(c, m);
    ADC(c, m.read(addr));
    return 4 + (extra ? 1 : 0);
  };
  t[0x79] = [](CPU& c, Memory& m)
  {
    auto const [extra, addr] = ABY(c, m);
    ADC(c, m.read(addr));
    return 4 + (extra ? 1 : 0);
  };
  t[0x61] = [](CPU& c, Memory& m)
  {
    ADC(c, m.read(IZX(c, m)));
    return 6;
  };
  t[0x71] = [](CPU& c, Memory& m)
  {
    auto const [extra, addr] = IZY(c, m);
    ADC(c, m.read(addr));
    return 5 + (extra ? 1 : 0);
  };

  // SBC
  t[0xE9] = [](CPU& c, Memory& m)
  {
    SBC(c, m.read(IMM(c, m)));
    return 2;
  };
  t[0xE5] = [](CPU& c, Memory& m) {
    SBC(c, m.read(ZP0(c, m)));
    return 3;
  };
  t[0xF5] = [](CPU& c, Memory& m)
  {
    SBC(c, m.read(ZPX(c, m)));
    return 4;
  };
  t[0xED] = [](CPU& c, Memory& m)
  {
    SBC(c, m.read(ABS(c, m)));
    return 4;
  };
  t[0xFD] = [](CPU& c, Memory& m)
  {
    auto const [extra, addr] = ABX(c, m);
    SBC(c, m.read(addr));
    return 4 + (extra ? 1 : 0);
  };
  t[0xF9] = [](CPU& c, Memory& m) {
    auto const [extra, addr] = ABY(c, m);
    SBC(c, m.read(addr));
    return 4 + (extra ? 1 : 0);
  };
  t[0xE1] = [](CPU& c, Memory& m)
  {
    SBC(c, m.read(IZX(c, m)));
    return 6;
  };
  t[0xF1] = [](CPU& c, Memory& m)
  {
    auto const [extra, addr] = IZY(c, m);
    SBC(c, m.read(addr));
    return 5 + (extra ? 1 : 0);
  };

  return t;
}

constexpr auto opcode_table = create_opcode_table();

}
#endif  // NES_OPCODES_HPP
