# Add Unit Test Executable
add_executable(nes_tests
  memory.test.cpp
  address_modes.test.cpp
  cpu.test.cpp
  instructions.test.cpp
  opcodes.test.cpp
)

target_link_libraries(nes_tests PRIVATE
  Catch2::Catch2
  Catch2::Catch2WithMain
  neslib
)

add_test(NAME nes_tests COMMAND nes_tests)
