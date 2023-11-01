#ifndef LIBCALC_HPP
#define LIBCALC_HPP

#include <concepts>
#include <cstdint>
#include <mutex>
#include <thread>

namespace Calculator {

/**
 * @concept Arithmetic
 * @brief A concept that checks whether a type is an arithmetic type.
 */
template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

/**
 * @concept Integral
 * @brief A concept that checks whether a type is an integral type.
 */
template <typename T>
concept Integral = std::integral<T>;

/**
 * @concept FloatingPoint
 * @brief A concept that checks whether a type is a floating-point type.
 */
template <typename T>
concept FloatingPoint = std::floating_point<T>;

/**
 * @class Calculator
 * @brief A simple calculator class for basic mathematical operations, and a
 * multithreaded factorial
 *
 * The Calculator class provides methods for performing common mathematical
 * operations such as addition, subtraction, multiplication, division, and
 * multithreaded factorial calculation. All the parameters are made auto for
 * automatic template generation and type inferrence flexibility.
 */
class Calculator {
public:
	Calculator();
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
	[[nodiscard]] constexpr auto add(const auto lhs, const auto rhs) noexcept
		-> auto
		requires Arithmetic<decltype(lhs)> && Arithmetic<decltype(rhs)>
	{
		return lhs + rhs;
	}

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
	[[nodiscard]] constexpr auto subtract(const auto lhs,
										  const auto rhs) noexcept -> auto
		requires Arithmetic<decltype(lhs)> && Arithmetic<decltype(rhs)>
	{
		return lhs - rhs;
	}

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
	[[nodiscard]] constexpr auto multiply(const auto lhs,
										  const auto rhs) noexcept -> auto
		requires Arithmetic<decltype(lhs)> && Arithmetic<decltype(rhs)>
	{
		return lhs * rhs;
	}

	/**
	 * @brief Divides two integers.
	 *
	 * This function takes two integers, `lhs` (divisor) and `rhs` (dividend),
	 * and returns the result of their division. If a number is divided by zero,
	 * an exception is thrown.
	 *
	 * @param lhs The divisor.
	 * @param rhs The dividend.
	 * @returns The result of dividing `lhs` by `rhs`.
	 */
	[[nodiscard]] constexpr auto divide(const auto lhs, const auto rhs) -> auto
		requires Arithmetic<decltype(lhs)> && Arithmetic<decltype(rhs)>
	{
		if (rhs == 0) {
			throw std::logic_error("Attempted division by zero");
		}
		return lhs / rhs;
	}

	/**
	 * @brief Calculates the factoral of an integer.
	 *
	 * This function takes an integer and calculates it's factorials using
	 * multiple threads using dynamic programming strategy
	 *
	 * @param num The number who's factorial is to be calculated
	 * @returns The factorial of `num`
	 */
	[[nodiscard]] auto factorial(const uint64_t num) -> uint64_t;

private:
	/**
	 * @brief Calculates the factorial within a specified range [start, end]
	 * using a for loop. The result is updated in a thread-safe manner.
	 *
	 * @param start The start of the range for which to calculate the
	 * factorial.
	 * @param end   The end of the range for which to calculate the
	 * factorial.
	 *
	 * This function calculates the factorial of the range [start, end] by
	 * iterating through the numbers in the range and multiplying them
	 * together. The result is then updated in a thread-safe manner using a
	 * mutex to prevent data races.
	 */
	auto calculate_factorial_range(const uint64_t start,
								   const uint64_t end) noexcept -> void;

	uint64_t result;
	std::mutex factorial_mutex;
};

} // namespace Calculator

#endif /* LIBCALC_HPP */