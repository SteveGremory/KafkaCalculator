#include <catch2/catch_test_macros.hpp>
#include <libcalc/libcalc.hpp>

TEST_CASE("Subtraction is computed", "[subtraction]") {
	auto calc = Calculator::Calculator();

	// Test case 1: Subtracting positive integers
	REQUIRE(calc.subtract(10, 5) == 5);

	// Test case 2: Subtracting a negative integer from a positive integer
	REQUIRE(calc.subtract(5, -3) == 8);

	// Test case 3: Subtracting a larger number from a smaller number
	REQUIRE(calc.subtract(3, 10) == -7);
}
