cmake_minimum_required(VERSION 3.10)
project(nes)

add_subdirectory(extern/SDL)

add_executable(nes
    src/nes.main.cpp
    # cpu.cpp
    # memory.cpp
)

# Set C++20 standard and enable extra warnings
target_compile_features(nes PRIVATE cxx_std_20)
target_compile_options(nes PRIVATE -Wall -Wextra)
target_link_libraries(nes SDL2::SDL2)
