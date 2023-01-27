#ifndef MOS6502_TIMED_EXECUTE_HPP
#define MOS6502_TIMED_EXECUTE_HPP
#include <thread>
#include <chrono>

namespace mos6502 {

/// Calls code_block(); at a frequency of HZ in loop. Fn signature is bool fn();
/** If code_block returns true, this function will return. */
template <
  int HZ,
  typename Fn,
  typename Clock_t = std::chrono::high_resolution_clock
>
auto timed_execute(
  Fn code_block
) -> void
{
  static_assert(HZ > 0);
  constexpr auto period = std::chrono::nanoseconds{1'000'000'000} / HZ;
  bool exit = false;
  while(!exit) {
    auto const end_at = Clock_t::now() + period;
    exit = code_block();
    std::this_thread::sleep_until(end_at);
  }
}

}  // namespace mos6502
#endif  // MOS6502_TIMED_EXECUTE_HPP
