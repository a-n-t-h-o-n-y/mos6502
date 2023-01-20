#include <nes/memory.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Initialized to zeros", "[memory]")
{
  auto memory = nes::Memory{};
  for (auto addr = std::uint16_t{0}; addr < 0xFFFF; ++addr) {
    CHECK(memory.read(addr) == std::uint8_t{0});
  }
  // avoid infinite loop
  CHECK(memory.read(0xFFFF) == std::uint8_t{0});
}

TEST_CASE("Write value and read", "[memory]")
{
  auto memory = nes::Memory{};
  memory.write(0x0010, 0x0F);
  CHECK(memory.read(0x0010 + (0x0800 * 0x0)) == 0x0F);
  CHECK(memory.read(0x0010 + (0x0800 * 0x1)) == 0x0F);
  CHECK(memory.read(0x0010 + (0x0800 * 0x2)) == 0x0F);
  CHECK(memory.read(0x0010 + (0x0800 * 0x3)) == 0x0F);

  memory.write(0x0010 + (0x0800 * 0x2), 0x38);
  CHECK(memory.read(0x0010 + (0x0800 * 0x0)) == 0x38);
  CHECK(memory.read(0x0010 + (0x0800 * 0x1)) == 0x38);
  CHECK(memory.read(0x0010 + (0x0800 * 0x2)) == 0x38);
  CHECK(memory.read(0x0010 + (0x0800 * 0x3)) == 0x38);
}
