#include "libcalc.hpp"

namespace Calculator {

[[nodiscard]] auto Calculator::add(const mpz_class& lhs,
								   const mpz_class& rhs) noexcept -> mpz_class {
	return lhs + rhs;
}

[[nodiscard]] auto Calculator::subtract(const mpz_class& lhs,
										const mpz_class& rhs) noexcept
	-> mpz_class {
	return lhs - rhs;
}

[[nodiscard]] auto Calculator::multiply(const mpz_class& lhs,
										const mpz_class& rhs) noexcept
	-> mpz_class {
	return lhs * rhs;
}

[[nodiscard]] auto Calculator::divide(const mpz_class& lhs,
									  const mpz_class& rhs) -> mpz_class {
	return lhs / rhs;
}

auto Calculator::calculate_factorial_range(const mpz_class& start,
										   const mpz_class& end) noexcept
	-> void {
	mpz_class local_result = 1;
	for (auto i = start; i <= end; ++i) {
		local_result *= i;
	}

	const std::lock_guard<std::mutex> fact_lock(this->factorial_mutex);
	this->result *= local_result;
}

[[nodiscard]] auto Calculator::factorial(const mpz_class& num) -> mpz_class {
	this->result = 1;

	auto threads = std::vector<std::thread>{};

	const auto num_threads = std::thread::hardware_concurrency();
	const auto step = num / num_threads;

	for (mpz_class i = 0; i < num_threads; ++i) {
		const mpz_class start = i * step + 1;
		const mpz_class end = (i == num_threads - 1) ? num : (i + 1) * step;

		threads.emplace_back(&Calculator::calculate_factorial_range, this,
							 start, end);
	}

	for (auto& thread : threads) {
		thread.join();
	}

	return this->result;
}
} // namespace Calculator
