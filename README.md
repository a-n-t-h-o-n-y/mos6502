# MOS 6502 Emulator ⌨️

MOS Technology 6502 Emulator in C++20.

Features:

- Supports Decimal Mode.
- Passes Klaus2m5 `6502_functional_test.bin` test.
- Does not support illegal opcodes.

## Building

- Clone the repository.
- Fetch git submodules (Catch2).
- Build with CMake.

## Usage

```cpp
#include <mos6502/byte_array.hpp>
#include <mos6502/opcodes.hpp>
#include <mos6502/execute.hpp>

using namespace mos6502;

int main() {

  ByteArray  mem   = load_binary("a_rom.bin");
  auto const table = create_opcode_table<ByteArray>();

  auto cpu = CPU{};
  cpu.PC   = 0x0400;

  auto prev = -1;
  while (prev != cpu.PC) {
    prev = cpu.PC;
    execute_next_opcode(table, cpu, mem); // Returns # of cycles used.
  }

  return 0;
}
```
