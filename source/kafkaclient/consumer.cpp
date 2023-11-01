#include <cppkafka/cppkafka.h>
#include <nlohmann/json.hpp>

#include <iostream>
#include <signal.h>

#include "kf.hpp"
#include <gmpxx.h>
#include <libcalc/libcalc.hpp>

static bool running = true;

auto main() -> int {
	// Create the initial objects
	auto calc = Calculator::Calculator();
	auto config =
		cppkafka::Configuration({{"metadata.broker.list", KAFKA_BROKER},
								 {"group.id", "consumer_group"},
								 {"enable.auto.commit", false}});

	// Create a Kafka consumer instance
	auto consumer = cppkafka::Consumer(config);

	// Subscribe to the topic
	consumer.subscribe({"testing_topic"});
	// Cleanup
	signal(SIGINT, [](int) { running = false; });

	// Print the assigned partitions on assignment
	consumer.set_assignment_callback(
		[](const cppkafka::TopicPartitionList& partitions) {
			std::cout << "Got assigned: " << partitions << std::endl;
		});

	// Print the revoked partitions on revocation
	consumer.set_revocation_callback(
		[](const cppkafka::TopicPartitionList& partitions) {
			std::cout << "Got revoked: " << partitions << std::endl;
		});

	// Poll for messages
	while (running) {

		cppkafka::Message msg = consumer.poll(std::chrono::milliseconds{100});
		// If we managed to get a message
		if (msg) {
			if (msg.get_error()) {
				// Ignore EOF notifications from rdkafka
				if (!msg.is_eof()) {
					std::cout << "[+] Received error notification: "
							  << msg.get_error() << std::endl;
				}
			} else {
				try {
					nlohmann::json json_obj =
						nlohmann::json::parse(msg.get_payload());

					// Access object properties
					Operations operation = json_obj["operation"];
					std::string op1_str = json_obj["op1"];
					std::string op2_str = json_obj["op2"];

					mpz_class op1{op1_str};
					mpz_class op2{op2_str};

					mpz_class result{};

					std::cout << "Operation: ";
					switch (operation) {
					case Operations::ADD: {
						std::cout << "Addition";
						result = calc.add(op1, op2);
						break;
					}
					case Operations::SUB: {
						std::cout << "Subtraction";
						result = calc.subtract(op1, op2);
						break;
					}
					case Operations::MUL: {
						std::cout << "Multiplication";
						result = calc.multiply(op1, op2);
						break;
					}
					case Operations::DIV: {
						std::cout << "Division";
						result = calc.divide(op1, op2);
						break;
					}
					case Operations::FCT: {
						std::cout << "Factorial";
						result = calc.factorial(op1);
						break;
					}
					default:
						break;
					};
					std::cout << " -> " << result.get_str() << std::endl;

				} catch (const std::exception& e) {
					// Catch and handle the exception
					std::cerr << "An exception occurred: " << e.what()
							  << std::endl;
				}
			}

			// Now commit the message
			consumer.commit(msg);
		}
	}

	return 0;
}