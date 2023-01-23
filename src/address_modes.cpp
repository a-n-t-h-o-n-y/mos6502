#include <nes/address_modes.hpp>

#include <cstdint>
#include <utility>

#include <nes/address.hpp>
#include <nes/byte.hpp>

namespace {
using namespace nes;

auto abs_indexed(
  CPU& cpu,
  Memory const& memory,
  Byte index
) -> std::pair<bool, Address>
{
  Address const abs = ABS(cpu, memory);
  if (((abs + index) & 0xFF00) != (abs & 0xFF00))
    return {true, abs + index};
  else
    return {false, abs + index};
}

/// Two's complement byte to two's complement word (2 Bytes).
auto sign_extend(Byte b) -> Address
{
  Address word = b;
  if (word & 0x0080)
    word |= 0xFF00;
  return word;
}

}  // namespace

namespace nes {

auto ACC(
  CPU const& cpu
) -> Byte { return cpu.AC; }

auto ABS(
  CPU& cpu,
  Memory const& memory
) -> Address
{
  Address const lo = memory.read(cpu.PC++);
  Address const hi = memory.read(cpu.PC++);

  return (hi << 8) | lo;
}

auto ABX(
  CPU& cpu,
  Memory const& memory
) -> std::pair<bool, Address>
{
  return ::abs_indexed(cpu, memory, cpu.X);
}

auto ABY(
  CPU& cpu,
  Memory const& memory
) -> std::pair<bool, Address>
{
  return ::abs_indexed(cpu, memory, cpu.Y);
}

auto IMM(
  CPU& cpu,
  Memory const& memory
) -> Byte
{
  return memory.read(cpu.PC++);
}

auto IND(
  CPU& cpu,
  Memory const& memory
) -> Address
{
  Address const abs = ABS(cpu, memory);
  if ((abs & 0x00FF) == 0xFF)
    return memory.read(abs & 0xFF00) << 8 | memory.read(abs);
  else
    return memory.read(abs + 1) << 8 | memory.read(abs);
}

auto IZX(
  CPU& cpu,
  Memory const& memory
) -> Address
{
  Address const zp = memory.read(cpu.PC++);
  Address const lo = memory.read(zp + cpu.X);
  Address const hi = memory.read(zp + cpu.X + 1);
  return (hi << 8) | lo;
}

auto IZY(
  CPU& cpu,
  Memory const& memory
) -> std::pair<bool, Address>
{
  Address const zp     = memory.read(cpu.PC++);
  Address const lo     = memory.read(zp);
  Address const hi     = memory.read(zp + 1);
  Address const result = ((hi << 8) | lo) + cpu.Y;
  if ((hi << 8) != (result & 0xFF00))
    return {true, result};
  else
    return {false, result};
}

auto REL(
  CPU& cpu,
  Memory const& memory
) -> Address
{
  Byte const bb = memory.read(cpu.PC++);
  return cpu.PC + sign_extend(bb);
}

auto ZP0(
  CPU& cpu,
  Memory const& memory
) -> Address
{
  return Address{memory.read(cpu.PC++)};
}

auto ZPX(
  CPU& cpu,
  Memory const& memory
) -> Address
{
  return (memory.read(cpu.PC++) + cpu.X) & 0x00FF;
}

auto ZPY(
  CPU& cpu,
  Memory const& memory
) -> Address
{
  return (memory.read(cpu.PC++) + cpu.Y) & 0x00FF;
}

}  // namespace nes
