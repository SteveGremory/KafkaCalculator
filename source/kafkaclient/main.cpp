#include <cppkafka/cppkafka.h>

auto main() -> int {
	// Create the config
	cppkafka::Configuration config = {
		{"metadata.broker.list", "127.0.0.1:9092"}};

	// Create the producer
	cppkafka::Producer producer(config);

	// Produce a message!
	std::string message = "hey there!";
	producer.produce(
		cppkafka::MessageBuilder("my_topic").partition(0).payload(message));
	producer.flush();

	return 0;
}