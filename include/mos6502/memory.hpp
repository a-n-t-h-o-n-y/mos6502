#ifndef MOS6502_MEMORY_HPP
#define MOS6502_MEMORY_HPP
#include <concepts>

#include <mos6502/address.hpp>
#include <mos6502/byte.hpp>

namespace mos6502 {

/// Byte store memory concept, for use with 6502 opcodes.
/** The emulated instructions will work with any type that implements this. */
template<typename M>
concept Memory = requires(M mem, Address addr, Byte val) {
    { mem.read(addr) } -> std::convertible_to<Byte>;
    { mem.write(addr, val) } -> std::same_as<void>;
};

}  // namespace mos6502
#endif//MOS6502_MEMORY_HPP
