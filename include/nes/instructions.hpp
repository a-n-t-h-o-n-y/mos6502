#ifndef NES_INSTRUCTIONS_HPP
#define NES_INSTRUCTIONS_HPP
#include <nes/cpu.hpp>
#include <nes/byte.hpp>

namespace nes {

/// Add Memory to Accumulator with Carry.
/** Always has the potential to add another cycle. */
auto ADC(
  CPU& cpu,
  Byte value
) -> void;

/// Subtract Memory from Accumulator with Borrow.
/** Always has the potential to add another cycle. */
auto SBC(
  CPU& cpu,
  Byte value
) -> void;


}  // namespace nes
#endif  // NES_INSTRUCTIONS_HPP
