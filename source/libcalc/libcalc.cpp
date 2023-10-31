#include "libcalc.hpp"

namespace Calculator {
Calculator::Calculator() : result(1) {}

auto Calculator::calculate_factorial_range(const uint64_t start,
										   const uint64_t end) noexcept
	-> void {
	uint64_t local_result = 1;
	for (auto i = start; i <= end; ++i) {
		local_result *= i;
	}

	const std::lock_guard<std::mutex> fact_lock(this->factorial_mutex);
	this->result *= local_result;
}

[[nodiscard]] auto Calculator::factorial(const uint64_t num) -> uint64_t {
	this->result = 1;

	auto threads = std::vector<std::thread>{};

	const uint64_t num_threads = std::thread::hardware_concurrency();
	const uint64_t step = floor(num / num_threads);

	for (uint64_t i = 0; i < num_threads; ++i) {
		const uint64_t start = i * step + 1;
		const uint64_t end = (i == num_threads - 1) ? num : (i + 1) * step;

		threads.emplace_back(&Calculator::calculate_factorial_range, this,
							 start, end);
	}

	for (auto& thread : threads) {
		thread.join();
	}

	return this->result;
}
} // namespace Calculator
