#include <catch2/catch_test_macros.hpp>
#include <libcalc/libcalc.hpp>

TEST_CASE("Factorial is computed", "[factorial]") {
	auto calc = Calculator::Calculator();

	// Test case 1: Factorial of 0 should be 1
	REQUIRE(calc.factorial(0) == 1);

	// Test case 2: Factorial of 1 should be 1
	REQUIRE(calc.factorial(1) == 1);

	// Test case 3: Factorial of a positive integer
	REQUIRE(calc.factorial(5) == 120);

	// Test case 4: Factorial of a larger number
	REQUIRE(calc.factorial(10) == 3628800);
}