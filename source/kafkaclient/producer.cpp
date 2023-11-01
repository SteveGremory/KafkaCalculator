#include <cppkafka/cppkafka.h>
#include <nlohmann/json.hpp>

#include <cstddef>
#include <iostream>

#include "kf.hpp"

auto main(int argc, char** argv) -> int {
	// Create a configuration for the producer
	cppkafka::Configuration config = {
		{"metadata.broker.list", KAFKA_BROKER},
	};

	// Create a Kafka producer instance
	cppkafka::Producer producer(config);

	// Create a message to send
	cppkafka::MessageBuilder message("testing_topic");

	if (argc < 4) {
		throw std::range_error("Not enough args");
	}

	// Create an object to be sent
	nlohmann::json json_obj = {{"operation", std::atoi(argv[1])},
							   {"op1", std::atoi(argv[2])},
							   {"op2", std::atoi(argv[3])}};

	std::string json_str = json_obj.dump();

	message.payload(json_str);

	// Send the message
	producer.produce(message);

	producer.flush();

	return 0;
}