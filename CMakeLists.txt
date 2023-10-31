cmake_minimum_required(VERSION 3.22)
project("Kafka Calculator")

# Set C++ options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Fetch dependancy: Catch2
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1 # or a later release
)
FetchContent_MakeAvailable(Catch2)

# Find clang tidy
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=*,-llvmlibc-*,-google-*,-fuchsia-*,-android-*,-altera-*,-abseil-*,-boost-*,-objc-*,-openmp-*,-zircon-*,-misc-include-cleaner,-readability-convert-member-functions-to-static")

# The calculator (header only) library
add_library(calc INTERFACE)
target_include_directories(calc
    INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Make the calc library use clang tidy in debug mode
set_target_properties(calc PROPERTIES
    CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
)

# Get all C++ test files
file(GLOB_RECURSE TESTS_SOURCES
    tests/*.cpp
)

# Tests
add_executable(tests ${TESTS_SOURCES})
target_link_libraries(tests
    PRIVATE Catch2::Catch2WithMain
    PRIVATE calc
)

# Enable some compiler/linker options.
set(ASAN_OPTIONS

    -fsanitize=address -fsanitize=pointer-compare
    -fsanitize=pointer-subtract -fsanitize=undefined
    -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow
)

target_compile_options(tests PUBLIC
    # Error options
    -Wall -Wextra -Werror

    # Stack pointer
    -fno-omit-frame-pointer

    # Add asan options
    ${ASAN_OPTIONS}

    $<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>
)
target_link_options(tests PUBLIC
    # Add asan options
    ${ASAN_OPTIONS}
    
    # Stack pointer
    -fno-omit-frame-pointer
)
