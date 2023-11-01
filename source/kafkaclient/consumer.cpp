#include <cppkafka/cppkafka.h>
#include <gmpxx.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <iostream>
#include <signal.h>

#include <kflib/kf.hpp>
#include <libcalc/libcalc.hpp>

static bool running = true;

auto main(int argc, char** argv) -> int {

	if (argc <= 1) {
		throw std::range_error("Not enough args");
	}

	// Future cleanup
	signal(SIGINT, [](int) { running = false; });

	// Create the initial objects
	auto calc = Calculator::Calculator();
	const auto json_config = KafkaClient::ConfigLoader::load_config(argv[1]);

	auto config = cppkafka::Configuration(
		{{"metadata.broker.list", json_config.kafka_broker},
		 {"group.id", json_config.group_id},
		 {"enable.auto.commit", false}});

	// Create a Kafka consumer instance
	auto consumer = cppkafka::Consumer(config);

	// Subscribe to the topic
	consumer.subscribe({json_config.kafka_topic});

	// Print the assigned partitions on assignment
	consumer.set_assignment_callback(
		[](const cppkafka::TopicPartitionList& partitions) {
			std::stringstream ss("Got assigned: ");
			ss << partitions;

			spdlog::info(ss.str());
		});

	// Print the revoked partitions on revocation
	consumer.set_revocation_callback(
		[](const cppkafka::TopicPartitionList& partitions) {
			std::stringstream ss("Got revoked: ");
			ss << partitions;

			spdlog::info(ss.str());
		});

	// Initialise all the variables only once
	nlohmann::json json_obj{};

	KafkaClient::Operations operation{};

	mpz_class op1{};
	mpz_class op2{};

	mpz_class result{};

	std::string operation_name{};

	// Poll for messages
	while (running) {
		cppkafka::Message msg =
			consumer.poll(std::chrono::milliseconds{json_config.poll_delay});

		// If we managed to get a message
		if (msg) {
			if (msg.get_error()) {
				// Ignore EOF notifications from rdkafka
				if (!msg.is_eof()) {
					spdlog::critical("Received error notification: {}",
									 msg.get_error().to_string());
				}
			} else {
				try {
					json_obj = nlohmann::json::parse(msg.get_payload());

					operation = json_obj["operation"];

					op1 = mpz_class{std::string(json_obj["op1"])};
					op2 = mpz_class{std::string(json_obj["op2"])};

					switch (operation) {
					case KafkaClient::Operations::ADD: {
						operation_name = "Addition(" + op1.get_str() + '+' +
										 op2.get_str() + ')';
						result = calc.add(op1, op2);

						break;
					}

					case KafkaClient::Operations::SUB: {
						operation_name = "Subtraction(" + op1.get_str() + '-' +
										 op2.get_str() + ')';
						result = calc.subtract(op1, op2);

						break;
					}

					case KafkaClient::Operations::MUL: {
						operation_name = "Multiplication(" + op1.get_str() +
										 '*' + op2.get_str() + ')';
						result = calc.multiply(op1, op2);

						break;
					}

					case KafkaClient::Operations::DIV: {
						operation_name = "Division(" + op1.get_str() + '/' +
										 op2.get_str() + ')';
						result = calc.divide(op1, op2);

						break;
					}

					case KafkaClient::Operations::FCT: {
						operation_name = "Factorial(" + op1.get_str() + ')';
						result = calc.factorial(op1);

						break;
					}

					default:

						break;
					};

					spdlog::info("Operation: {} -> {}", operation_name,
								 result.get_str());

				} catch (const std::exception& e) {
					// Catch and handle the exception
					spdlog::critical("An exception occurred: {}", e.what());
				}
			}

			// Reset the result after every calculation
			result = 0;

			// Now commit the message
			consumer.commit(msg);
		}
	}

	return 0;
}