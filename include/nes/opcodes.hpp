#ifndef NES_OPCODES_HPP
#define NES_OPCODES_HPP
#include <nes/address_modes.hpp>
#include <nes/instructions.hpp>

namespace nes {

// TODO Use the addressing modes and instructions to create an array of 256
// opcodes that can be indexed on and then called, the array stores function
// pointers, the functions are inline lambdas that call on the addressing mode
// and forward the info to the instruction, they return the total cycle count of
// the opcode.

  // InstructionTable[0xB0] = new CpuInstructionOpBCS(*this, AddressingMode::Relative);
  // Initialize with nullptrs and then assign each address like:
  // table[0xB0] = [](cpu, memory){LDA(REL()); return 5;}
  // this way you explicitly see which opcode is being used.
  // This is better than a huge table, and you might be able to initialize it in
  // a constexpr fn. at least try it, it isn't a major init startup cost at all
  // otherwise.

  // table[0xB0] = [](auto& cpu, auto const& memory)
  //   {
  //     cycle1, addr = REL(cpu, memory);
  //     value  = memory.read(addr);
  //     cycle2 = LDA(cpu, value);
  //     return 5 + (cycle1 & cycle2);
  //   }

}
#endif  // NES_OPCODES_HPP
