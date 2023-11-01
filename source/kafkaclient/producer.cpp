#include <cppkafka/cppkafka.h>
#include <nlohmann/json.hpp>

#include <cstddef>
#include <filesystem>

#include <kflib/kf.hpp>

auto main(int argc, char** argv) -> int {
	if (argc < 5) {
		throw std::range_error("Not enough args: path, operation, op1, op2");
	}

	// Create a configuration for the producer
	const auto json_config = KafkaClient::ConfigLoader::load_config(argv[1]);
	cppkafka::Configuration config = {
		{"metadata.broker.list", json_config.kafka_broker},
	};

	// Create a Kafka producer instance
	cppkafka::Producer producer(config);

	// Create a message to send
	cppkafka::MessageBuilder message({json_config.kafka_topic});

	// Create an object to be sent
	nlohmann::json json_obj = {
		{"operation", std::atoi(argv[2])},
		{"op1", argv[3]},
		{"op2", argv[4]},
	};
	std::string json_str = json_obj.dump();

	message.payload(json_str);

	producer.produce(message);

	// Send the message
	producer.flush();

	return 0;
}