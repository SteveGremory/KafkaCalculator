#include <catch2/catch_test_macros.hpp>
#include <libcalc/libcalc.hpp>

#include <iostream>

TEST_CASE("Factorial is computed", "[factorial]") {
	auto calc = Calculator::Calculator();
	bool res{false};

	// Test case 1: Factorial of 0 should be 1
	res = calc.factorial(0) == 1;
	REQUIRE(res == true);

	// Test case 2: Factorial of 1 should be 1
	res = calc.factorial(1) == 1;
	REQUIRE(res == true);

	// Test case 3: Factorial of a positive integer
	res = calc.factorial(5) == 120;
	REQUIRE(res == true);

	// Test case 4: Factorial of a larger number
	res = calc.factorial(10) == 3628800;
	REQUIRE(res == true);
}