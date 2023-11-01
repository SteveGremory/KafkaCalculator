# Kafka Calculator

A basic calculator that works with kafka.

# How does this work?

## Calculator

The calculator class has basic math functions that work with Integers. The GNU MP Library is used under the hood for multiple precision arithmetic. The class has an overloaded function for all operations to differentiate between strings and numbers that uses C++20 concepts to check what overload to use at compile time.

The factorial function uses all available threads to divide it into `num / num_threads` steps which are then separately calculated by separate threads.

# Build instructions

## Dependencies

To compile the program, you need to install **CMake** and the following libraries:

- **GMP** (The GNU Multiple Precision Arithmetic Library) for high precision math
- **Boost**, as it's a dependency for CppKafka
- **librdkafka**, which is also a dependency for CppKafka

All the other required libraries are automatically fetched and built using CMake.

## Compilation

To generate the makefile(s) for the project, run `cmake . -Bbuild` from the root of the project, then change the current working directly to `build/` and run the generator that was used by CMake, on UNIX-like systems, it's mostly either Make or Ninja.

## Running

To run the program, you must modify the `settings.json` file stored in the same folder as the executable to suit your environment. Here, you need to set:

- The Kafka broker (kafka_broker)
- The Kafka topic (kafka_topic)
- Event polling delay (poll_delay)
- Maximum queue buffering time (queue_max_time)
- Consumer group id (group_id)
