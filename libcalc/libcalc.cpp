#include "libcalc.hpp"

#include <stdexcept>
#include <string>

namespace Calculator {

auto Calculator::add(int64_t lhs, int64_t rhs) -> int64_t { return lhs + rhs; }

auto Calculator::subtract(int64_t lhs, int64_t rhs) -> int64_t {
	return lhs - rhs;
}

auto Calculator::multiply(int64_t lhs, int64_t rhs) -> int64_t {
	return lhs * rhs;
}

auto Calculator::divide(int64_t lhs, int64_t rhs) -> int64_t {
	return lhs / rhs;
}

auto Calculator::factorial(int64_t num) -> int64_t {
	throw std::logic_error("Un-Implemented function: " + std::to_string(num));
}

} // namespace Calculator
