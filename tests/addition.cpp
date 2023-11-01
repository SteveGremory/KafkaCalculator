#include <catch2/catch_test_macros.hpp>
#include <libcalc/libcalc.hpp>

#include <limits>

TEST_CASE("Addition is computed", "[addition]") {
	auto calc = Calculator::Calculator();

	bool res{false};

	// Test case 1: Adding positive integers
	res = calc.add(2, 4) == 6;
	REQUIRE(res == true);

	// Test case 2: Adding positive integers with a larger result
	res = calc.add(INT32_MAX, INT32_MAX) == 4294967294;
	REQUIRE(res == true);

	// Test case 3: Adding a positive and a negative integer
	res = calc.add(-5, 3) == -2;
	REQUIRE(res == true);
}
