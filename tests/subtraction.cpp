#include <catch2/catch_test_macros.hpp>
#include <libcalc/libcalc.hpp>

TEST_CASE("Subtraction is computed", "[subtraction]") {
	auto calc = Calculator::Calculator();

	bool res{false};

	// Test case 1: Subtracting positive integers
	res = calc.subtract(10, 5) == 5;
	REQUIRE(res == true);

	// Test case 2: Subtracting a negative integer from a positive integer
	res = calc.subtract(5, -3) == 8;
	REQUIRE(res == true);

	// Test case 3: Subtracting a larger number from a smaller number
	res = calc.subtract(3, 10) == -7;
	REQUIRE(res == true);
}
