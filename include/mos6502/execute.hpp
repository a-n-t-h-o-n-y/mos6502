#ifndef MOS6502_EXECUTE_HPP
#define MOS6502_EXECUTE_HPP
#include <chrono>
#include <concepts>
#include <cstdint>
#include <thread>

#include <mos6502/cpu.hpp>
#include <mos6502/memory.hpp>
#include <mos6502/opcodes.hpp>

namespace mos6502 {

/// Return the opcode at cpu.PC and increment cpu.PC.
[[nodiscard]]
auto read_opcode(
  CPU& cpu,
  Memory auto& mem
) -> Byte
{
  return mem.read(cpu.PC++);
}

/// Executes the opcode at mem.read(cpu.PC). Returns number of cycles used.
template <Memory M>
auto execute_next_opcode(
  OpTable<M> const& table,
  CPU& cpu,
  Memory auto& mem
) -> int
{
  return table[read_opcode(cpu, mem)](cpu, mem);
}

template <typename T>
concept Predicate = requires(T t) {
  { t() } -> std::same_as<bool>;
};

/// Calls code_block(); at a frequency of HZ in loop. Fn signature is bool fn();
/** If code_block returns true, this function will return. */
template <
  std::uint32_t HZ,
  Predicate Fn,
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
#endif //MOS6502_EXECUTE_HPP
