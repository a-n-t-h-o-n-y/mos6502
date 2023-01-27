# Add Unit Test Executable
add_executable(mos6502_tests
  addressing_modes.test.cpp
  cpu.test.cpp
  instructions.test.cpp
  opcodes.test.cpp
)

target_link_libraries(mos6502_tests PRIVATE
  Catch2::Catch2
  Catch2::Catch2WithMain
  mos6502
)

add_test(NAME mos6502_tests COMMAND mos6502_tests)
