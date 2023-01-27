cmake_minimum_required(VERSION 3.20)
project(mos6502)
enable_testing()

# Create mos6502 Library
add_library(mos6502
  src/address_modes.cpp
  src/cpu.cpp
  src/instructions.cpp
)
target_include_directories(mos6502 PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include/
)
target_compile_features(mos6502 PUBLIC
  cxx_std_20
)
target_compile_options(mos6502 PUBLIC
  -Wall
  -Wextra
  -Wshadow
  -Wconversion
)

# Add the Catch2 Library
add_subdirectory(extern/Catch2)

# Add Tests
add_subdirectory(tests)
