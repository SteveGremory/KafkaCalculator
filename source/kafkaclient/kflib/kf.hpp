#ifndef KF_HPP
#define KF_HPP

#include <cstdint>
#include <filesystem>
#include <iostream>

namespace KafkaClient {

/**
 * @struct ConfigLoader
 * @brief A utility for loading Kafka configuration.
 */
struct ConfigLoader {
	ConfigLoader() = default;
	~ConfigLoader() = default;

	/**
	 * @struct KafkaConfig
	 * @brief Represents Kafka configuration settings.
	 */
	struct KafkaConfig {
		std::string kafka_broker; ///< Kafka broker address.
		std::string kafka_topic;  ///< Kafka topic name.
		std::string group_id;	  ///< Kafka consumer group ID.

		uint32_t poll_delay;	 ///< Polling delay in milliseconds.
		uint32_t queue_max_time; ///< Maximum queue time in milliseconds.
		std::string logfile;	 ///< Path to the logfile
	};

	/**
	 * @brief Load Kafka configuration from a file.
	 * @param path The path to the configuration file.
	 * @return The loaded Kafka configuration.
	 */
	static auto load_config(const std::filesystem::path& path)
		-> ConfigLoader::KafkaConfig;
};

// All the operations supported
// by the Calculator class
enum class Operations {
	ADD = 0,
	SUB,
	MUL,
	DIV,
	FCT,
};

} // namespace KafkaClient

#endif /* KF_HPP */