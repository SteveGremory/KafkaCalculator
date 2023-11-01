#include <catch2/catch_test_macros.hpp>
#include <libcalc/libcalc.hpp>

TEST_CASE("Multiplication is computed", "[multiplication]") {
	auto calc = Calculator::Calculator();

	bool res{false};

	// Test case 1: Multiplying positive integers
	res = calc.multiply(2, 4) == 8;
	REQUIRE(res == true);

	// Test case 2: Multiplying a positive and a negative integer
	res = calc.multiply(-5, 3) == -15;
	REQUIRE(res == true);

	// Test case 3: Multiplying zero by any number
	res = calc.multiply(0, 100) == 0;
	REQUIRE(res == true);
}