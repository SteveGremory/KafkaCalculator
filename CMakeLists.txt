cmake_minimum_required(VERSION 3.22)
project("Kafka Calculator")

# Set C++ options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
# Fetch dependancy: Catch2
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.0.1
)

# Fetch dependancy for CppKafka: librdkafka
set(RDKAFKA_BUILD_EXAMPLES OFF ON CACHE INTERNAL "Turn off examples")
set(RDKAFKA_BUILD_TESTS OFF CACHE INTERNAL "Turn off tests")
FetchContent_Declare(
    librdkafka
    GIT_REPOSITORY https://github.com/confluentinc/librdkafka
    GIT_TAG        v2.3.0
)

# Fetch dependancy: CppKafka
set(CPPKAFKA_DISABLE_EXAMPLES ON CACHE INTERNAL "Turn off examples")
set(CPPKAFKA_DISABLE_TESTS ON CACHE INTERNAL "Turn off tests")
FetchContent_Declare(
    CppKafka
    GIT_REPOSITORY https://github.com/mfontanini/cppkafka.git
    GIT_TAG        v0.3.1
)


FetchContent_MakeAvailable(Catch2)
FetchContent_MakeAvailable(librdkafka)
FetchContent_MakeAvailable(CppKafka)

# Find clang tidy
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=*,-llvmlibc-*,-google-*,-fuchsia-*,-android-*,-altera-*,-abseil-*,-boost-*,-objc-*,-openmp-*,-zircon-*,-misc-include-cleaner,-readability-convert-member-functions-to-static")

# Setup the libcalc library
file(GLOB CALCULATOR_SOURCES source/libcalc/*.cpp)
add_library(calc ${CALCULATOR_SOURCES})
set_target_properties(calc PROPERTIES
    CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
)
target_include_directories(calc
    PUBLIC source/
)

# Setup the Kafka client executable
file(GLOB KAFKACLIENT_SOURCES source/kafkaclient/*.cpp)
add_executable(kafka-client ${KAFKACLIENT_SOURCES})
set_target_properties(kafka-client PROPERTIES
    CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
)
target_include_directories(kafka-client
    PUBLIC source/
)
target_link_libraries(kafka-client PUBLIC
    calc
    rdkafka
    cppkafka
)

# Enable some compiler/linker options.
set(ASAN_OPTIONS
    -fsanitize=address -fsanitize=pointer-compare
    -fsanitize=pointer-subtract -fsanitize=undefined
    -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow
)

target_compile_options(calc PUBLIC
    # Error options
    -Wall -Wextra
    # Stack pointer
    -fno-omit-frame-pointer
    # Add asan options
    ${ASAN_OPTIONS}
    # Debug options
    $<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>
)
target_link_options(calc PUBLIC
    # Add asan options
    ${ASAN_OPTIONS}
    # Stack pointer
    -fno-omit-frame-pointer
)

target_compile_options(kafka-client PUBLIC
    # Error options
    -Wall -Wextra
    # Stack pointer
    -fno-omit-frame-pointer
    # Add asan options
    ${ASAN_OPTIONS}
    # Debug options
    $<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>
)
target_link_options(kafka-client PUBLIC
    # Add asan options
    ${ASAN_OPTIONS}
    # Stack pointer
    -fno-omit-frame-pointer
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

