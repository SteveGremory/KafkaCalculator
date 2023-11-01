#include <cppkafka/cppkafka.h>
#include <gmpxx.h>
#include <nlohmann/json.hpp>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
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

	// Create the initial objects
	auto calc = Calculator::Calculator();
	const auto json_config = KafkaClient::ConfigLoader::load_config(argv[1]);

	// Create a logger with console and file sinks
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto basic_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
		json_config.logfile);
	std::vector<spdlog::sink_ptr> sinks{console_sink, basic_sink};
	auto logger =
		std::make_shared<spdlog::logger>("main", sinks.begin(), sinks.end());

	// Future cleanup
	signal(SIGINT, [](int) { running = false; });

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
		[&logger](const cppkafka::TopicPartitionList& partitions) {
			std::stringstream ss("Got assigned: ");
			ss << partitions;

			logger->info(ss.str());
		});

	// Print the revoked partitions on revocation
	consumer.set_revocation_callback(
		[&logger](const cppkafka::TopicPartitionList& partitions) {
			std::stringstream ss("Got revoked: ");
			ss << partitions;

			logger->info(ss.str());
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
					logger->critical("Received error notification: {}",
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

					logger->info("Operation: {} -> {}", operation_name,
								 result.get_str());

				} catch (const std::exception& e) {
					// Catch and handle the exception
					logger->critical("An exception occurred: {}", e.what());
				}
			}

			// Reset the result after every calculation
			result = 0;

			// Now commit the message
			consumer.commit(msg);
			// and write to the log
			logger->flush();
		}
	}

	return 0;
}