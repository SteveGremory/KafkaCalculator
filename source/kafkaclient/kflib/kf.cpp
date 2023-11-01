#include "kf.hpp"

#include <array>
#include <fstream>
#include <nlohmann/json.hpp>

namespace KafkaClient {
auto ConfigLoader::load_config(const std::filesystem::path& path)
	-> ConfigLoader::KafkaConfig {
	// Load in the file and parse the JSON
	nlohmann::json json_obj{};

	try {
		std::ifstream config_file(path);
		json_obj = nlohmann::json::parse(config_file);
	} catch (...) {
		throw;
	}

	// Make sure everything is present
	const std::array<std::string, 6> required_keys = {
		"kafka_broker", "kafka_topic",	  "group_id",
		"poll_delay",	"queue_max_time", "logfile",
	};

	for (const std::string& key : required_keys) {
		if (!json_obj.contains(key)) {
			throw std::runtime_error("Key: " + key +
									 " was NOT found in the config file");
		}
	}

	// Give me RVO.
	return ConfigLoader::KafkaConfig{
		json_obj["kafka_broker"],	json_obj["kafka_topic"],
		json_obj["group_id"],		json_obj["poll_delay"],
		json_obj["queue_max_time"], json_obj["logfile"],
	};
}
} // namespace KafkaClient