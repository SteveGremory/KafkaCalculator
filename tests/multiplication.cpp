#include <catch2/catch_test_macros.hpp>
#include <libcalc/libcalc.hpp>

TEST_CASE("Multiplication is computed", "[multiplication]") {
	auto calc = Calculator::Calculator();

	// Test case 1: Multiplying positive integers
	REQUIRE(calc.multiply(2, 4) == 8);

	// Test case 2: Multiplying a positive and a negative integer
	REQUIRE(calc.multiply(-5, 3) == -15);

	// Test case 3: Multiplying zero by any number
	REQUIRE(calc.multiply(0, 100) == 0);
}