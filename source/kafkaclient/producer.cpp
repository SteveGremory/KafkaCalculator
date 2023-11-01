#include <boost/program_options.hpp>
#include <cppkafka/cppkafka.h>
#include <nlohmann/json.hpp>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <cstddef>
#include <filesystem>
#include <memory>
#include <unordered_map>

#include <kflib/kf.hpp>

namespace po = boost::program_options;

auto main(int argc, char** argv) -> int {
	std::string config_path{}, operation_arg{}, op1{}, op2{};
	KafkaClient::Operations operation{};

	// Boost argparser
	po::options_description desc(
		"\nAvailable operations: [add, sub, mul, div, fact].\n"
		"Invocation : <operation> <op1> <op2> --config_path <filepath> "
		"\nAgruments");
	desc.add_options()("help", "Display usage information")(
		"Operation", po::value<std::string>(&operation_arg)->required(),
		"The mathematical operation to perform")(
		"OP1", po::value<std::string>(&op1)->required(), "The first operand")(
		"OP2", po::value<std::string>(&op2)->required(), "The second operand")(
		"config_path", po::value<std::string>(&config_path)->required(),
		"Path to the JSON configuration file");

	// Create a positional_options_description and add the positional
	// arguments
	po::positional_options_description positional{};
	positional.add("Operation", 1);
	positional.add("OP1", 1);
	positional.add("OP2", 1);

	po::variables_map vm{};

	try {
		po::store(po::command_line_parser(argc, argv)
					  .options(desc)
					  .positional(positional)
					  .run(),
				  vm);

		po::notify(vm);

		if (vm.count("help")) {
			// Display usage information and exit
			std::cout << desc << std::endl;
			return 0;
		}

		// Continue with the rest of your program
		// 'operation', 'op1', and 'op2' are now accessed as regular variables
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	// Define a mapping between operation strings and their enum values
	const std::unordered_map<std::string, KafkaClient::Operations>
		operationMap = {
			{"add", KafkaClient::Operations::ADD},
			{"sub", KafkaClient::Operations::SUB},
			{"mul", KafkaClient::Operations::MUL},
			{"div", KafkaClient::Operations::DIV},
			{"fact", KafkaClient::Operations::FCT},
		};

	// Check for the operation
	if (operationMap.find(operation_arg) != operationMap.end()) {
		operation = operationMap.at(operation_arg);
	} else {
		std::cerr << "Invalid operation argument provided" << std::endl;
		return 1;
	}

	// Create a configuration for the producer
	const auto json_config =
		KafkaClient::ConfigLoader::load_config(config_path);
	cppkafka::Configuration config = {
		{"metadata.broker.list", json_config.kafka_broker},
	};

	// Create a Kafka producer instance
	cppkafka::Producer producer(config);

	// Create a message to send
	cppkafka::MessageBuilder message({json_config.kafka_topic});

	// Create an object to be sent
	nlohmann::json json_obj = {
		{"operation", operation},
		{"op1", op1},
		{"op2", op2},
	};
	std::string json_str = json_obj.dump();

	message.payload(json_str);

	producer.produce(message);

	// Send the message
	producer.flush();

	return 0;
}