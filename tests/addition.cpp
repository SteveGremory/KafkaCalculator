#include <catch2/catch_test_macros.hpp>
#include <libcalc/libcalc.hpp>

#include <limits>

// Max int with type info
constexpr int64_t INT32_MAX_T = INT32_MAX;

TEST_CASE("Addition is computed", "[addition]") {
	auto calc = Calculator::Calculator();

	// Test case 1: Adding positive integers
	REQUIRE(calc.add(2, 4) == 6);

	// Test case 2: Adding positive integers with a larger result
	REQUIRE(calc.add(INT32_MAX_T, INT32_MAX_T) == 4294967294);

	// Test case 3: Adding a positive and a negative integer
	REQUIRE(calc.add(-5, 3) == -2);
}
