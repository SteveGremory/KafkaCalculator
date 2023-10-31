#ifndef LIBCALC_HPP
#define LIBCALC_HPP

#include <cstdint>

namespace Calculator {
class Calculator {
public:
	Calculator() = default;
	~Calculator() = default;

	/**
	 * @brief Adds two integers.
	 *
	 * This function takes two integers, `lhs` and `rhs`, and returns their sum.
	 *
	 * @param lhs The number to which `rhs` is added.
	 * @param rhs The number being added.
	 * @returns The sum of `lhs` and `rhs`.
	 */
	auto add(int64_t lhs, int64_t rhs) -> int64_t;

	/**
	 * @brief Subtracts two integers.
	 *
	 * This function takes two integers, `lhs` and `rhs`, and returns their
	 * difference.
	 *
	 * @param lhs The number from which `rhs` is subtracted.
	 * @param rhs The number being subtracted.
	 * @returns The difference between `lhs` and `rhs`.
	 */
	auto subtract(int64_t lhs, int64_t rhs) -> int64_t;

	/**
	 * @brief Multiplies two integers.
	 *
	 * This function takes two integers, `lhs` and `rhs`, and returns their
	 * product.
	 *
	 * @param lhs The number to be multiplied.
	 * @param rhs The number being multiplied.
	 * @returns The product of `lhs` and `rhs`.
	 */
	auto multiply(int64_t lhs, int64_t rhs) -> int64_t;

	/**
	 * @brief Divides two integers.
	 *
	 * This function takes two integers, `lhs` (divisor) and `rhs` (dividend),
	 * and returns the result of their division.
	 *
	 * @param lhs The divisor.
	 * @param rhs The dividend.
	 * @returns The result of dividing `lhs` by `rhs`.
	 */
	auto divide(int64_t lhs, int64_t rhs) -> int64_t;

	/**
	 * @brief Calculates the factoral of an integer.
	 *
	 * This function takes an integer and calculates it's factorials using
	 * multiple threads using dynamic programming strategy
	 *
	 * @param num The number who's factorial is to be calculated
	 * @returns The factorial of `num`
	 */
	auto factorial(int64_t num) -> int64_t;

private:
};

} // namespace Calculator

#endif // LIBCALC_HPP