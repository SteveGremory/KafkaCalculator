#include <catch2/catch_test_macros.hpp>
#include <libcalc/libcalc.hpp>

TEST_CASE("Division is computed", "[division]") {
	auto calc = Calculator::Calculator();

	bool res{false};

	// Test case 1: Dividing positive integers with no remainder
	res = calc.divide(10, 2) == 5;
	REQUIRE(res == true);

	// Test case 2: Dividing positive integers with a remainder
	res = calc.divide(10, 3) == 3;
	REQUIRE(res == true);

	// Test case 3: Dividing a negative integer by a positive integer
	res = calc.divide(-8, 2) == -4;
	REQUIRE(res == true);

	// Test case 4: Dividing zero by any number
	res = calc.divide(0, 42) == 0;
	REQUIRE(res == true);

	// Test case 5: Dividing by zero should not
	REQUIRE_THROWS(calc.divide(42, 0));
}
