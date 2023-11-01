#ifndef KF_HPP
#define KF_HPP

constexpr auto KAFKA_BROKER = "127.0.0.1:56478";

enum class Operations {
	ADD = 0,
	SUB,
	MUL,
	DIV,
	FCT,
};

#endif /* KF_HPP */