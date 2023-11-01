#ifndef KF_HPP
#define KF_HPP

#include <cstdint>
#include <filesystem>
#include <iostream>

namespace KafkaClient {

struct ConfigLoader {
	ConfigLoader() = default;
	~ConfigLoader() = default;

	struct KafkaConfig {
		std::string kafka_broker;
		std::string kafka_topic;
		std::string group_id;

		uint32_t poll_delay;
		uint32_t queue_max_time;
	};

	static auto load_config(const std::filesystem::path& path)
		-> ConfigLoader::KafkaConfig;
};

enum class Operations {
	ADD = 0,
	SUB,
	MUL,
	DIV,
	FCT,
};

} // namespace KafkaClient

#endif /* KF_HPP */