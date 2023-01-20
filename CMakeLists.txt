cmake_minimum_required(VERSION 3.20)
project(nes)
enable_testing()

# Turn off SDL Shared Libraries and Add SDL Subdirectory
set(SDL_SHARED_ENABLED_BY_DEFAULT OFF)
add_subdirectory(extern/SDL)

# Add NES Library
add_library(neslib
    src/memory.cpp
)
target_include_directories(neslib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)
target_compile_features(neslib PUBLIC
    cxx_std_20
)
target_compile_options(neslib PUBLIC
    -Wall
    -Wextra
    -Wshadow
)
target_link_libraries(neslib PUBLIC
    SDL2::SDL2
)


# Add NES Executable Target
add_executable(nes
    src/nes.main.cpp
)

# Enable LTO for the nes Target
include(CheckIPOSupported)
check_ipo_supported(RESULT lto_supported)
if(lto_supported)
    set_target_properties(neslib PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_target_properties(neslib PROPERTIES LINK_FLAGS -fuse-linker-plugin)
    set_target_properties(nes PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_target_properties(nes PROPERTIES LINK_FLAGS -fuse-linker-plugin)
endif()

target_link_libraries(nes PRIVATE
    neslib
)

# Add the Catch2 Library
add_subdirectory(extern/Catch2)

# Add NES Tests
add_subdirectory(tests)
