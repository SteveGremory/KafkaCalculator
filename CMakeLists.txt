cmake_minimum_required(VERSION 3.22)
project("Kafka Calculator")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

# Set C++ options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project options
option(USE_CLANG_TIDY OFF "An option to turn off clang-tidy")
find_package(Boost COMPONENTS program_options REQUIRED)

# Fetching all the deps
include(FetchContent)
# Fetch dependancy: Catch2
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.4.0
)

# Fetch dependancy: Spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG        v1.12.0
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

# Fetch dependancy: JSON Library
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)

FetchContent_MakeAvailable(Catch2)
FetchContent_MakeAvailable(spdlog)
FetchContent_MakeAvailable(librdkafka)
FetchContent_MakeAvailable(CppKafka)
FetchContent_MakeAvailable(json)

# Dependancy: GMP for high precision math
find_package(GMP REQUIRED)

# Setup the libcalc library
file(GLOB CALCULATOR_SOURCES source/libcalc/*.cpp)
add_library(calc ${CALCULATOR_SOURCES})
target_include_directories(calc PUBLIC source/)
target_link_libraries(calc PUBLIC GMP::GMP spdlog::spdlog)

# Setup the Kafka consumer/producer executable
set(KAFKA_EXEC_LIBS
    calc rdkafka cppkafka GMP::GMP
    nlohmann_json::nlohmann_json spdlog::spdlog
)

# Common functions
add_library(kflib source/kafkaclient/kflib/kf.cpp)
target_include_directories(kflib PUBLIC source/kafkaclient)
target_link_libraries(kflib PUBLIC nlohmann_json::nlohmann_json)

add_executable(kafka-producer source/kafkaclient/producer.cpp)
target_include_directories(kafka-producer PUBLIC ${Boost_INCLUDE_DIR} source/)
target_link_libraries(kafka-producer PUBLIC ${KAFKA_EXEC_LIBS} ${Boost_LIBRARIES} kflib)

add_executable(kafka-consumer source/kafkaclient/consumer.cpp)
target_include_directories(kafka-consumer PUBLIC ${Boost_INCLUDE_DIR} source/)
target_link_libraries(kafka-consumer PUBLIC ${KAFKA_EXEC_LIBS} ${Boost_LIBRARIES} kflib)


# Turn clang-tidy off/on
if (USE_CLANG_TIDY)
    # Find clang tidy
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
    set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=*,-llvmlibc-*,-google-*,-fuchsia-*,-android-*,-altera-*,-abseil-*,-boost-*,-objc-*,-openmp-*,-zircon-*,-misc-include-cleaner,-readability-convert-member-functions-to-static")

    set_target_properties(calc PROPERTIES
        CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
    )
    set_target_properties(kafka-producer PROPERTIES
        CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
    )
    set_target_properties(kafka-consumer PROPERTIES
        CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
    )

endif()

# Set compiler flags
set(COMPILER_FLAGS
    # All the warnings
    -Wall -Wextra
    # Except for the ones that aren't mine
    -Wno-deprecated -Wno-unused-parameter
    # Add asan options
    -fsanitize=address -fsanitize=pointer-compare
    -fsanitize=pointer-subtract -fsanitize=undefined
    -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow
    # Stack pointer
    -fno-omit-frame-pointer
)

target_compile_options(calc PUBLIC ${COMPILER_FLAGS})
target_link_options(calc PUBLIC ${COMPILER_FLAGS})

target_compile_options(kafka-producer PUBLIC ${COMPILER_FLAGS})
target_link_options(kafka-producer PUBLIC ${COMPILER_FLAGS})

# Get all C++ test files
file(GLOB_RECURSE TESTS_SOURCES
    tests/*.cpp
)

# Tests
add_executable(tests ${TESTS_SOURCES})
target_link_libraries(tests
    PRIVATE Catch2::Catch2WithMain
    PRIVATE calc
    PRIVATE rdkafka
    PRIVATE cppkafka
)

