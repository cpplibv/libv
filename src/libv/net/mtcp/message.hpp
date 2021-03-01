// Project: libv.net, File: src/libv/net/mtcp/message.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/utility/byte_swap.hpp>
#include <libv/utility/enum.hpp>
#include <libv/utility/storage_size.hpp>
// std
#include <cstdint>
#include <string>
//#include <vector>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

using MessageBody = std::string;
//using MessageBody = std::vector<std::byte>; // TODO P2: Switch to std::vector<std::byte> | or maybe dont

// -------------------------------------------------------------------------------------------------

constexpr auto MTCP_MESSAGE_MAX_RESERVE = libv::MB(1);
constexpr auto MTCP_MESSAGE_MAX_SIZE = std::min(static_cast<int64_t>(std::numeric_limits<uint32_t>::max()), libv::MB(4));

// -------------------------------------------------------------------------------------------------

enum class MessageType : uint8_t {
	message = 0,
//	mtcp_rate_limit_read = 1,
//	mtcp_rate_limit_write = 2,
//	mtcp_rate_limit_rw = 3,
//	mtcp_version = ,
//	mtcp_reconnect_request = 4,
//	mtcp_reconnect_reject = 5,
//	mtcp_reconnect_accept = 6,
};

// -------------------------------------------------------------------------------------------------

struct MessageHeader {
private:
	uint64_t network_type : 8;
	uint64_t _reserved : 8;
	uint64_t network_channel : 16;
	uint64_t network_size : 32;

public:
	explicit constexpr inline MessageHeader(
			MessageType type = MessageType::message,
			uint16_t channel = 0,
			uint32_t size = 0) noexcept :
			network_type(libv::host_to_network(libv::to_value(type))),
			_reserved(0),
			network_channel(libv::host_to_network(channel)),
			network_size(libv::host_to_network(size)) {}

public:
	[[nodiscard]] inline const std::byte* header_data() const noexcept {
		return reinterpret_cast<const std::byte*>(this);
	}
	[[nodiscard]] inline std::byte* header_data() noexcept {
		return reinterpret_cast<std::byte*>(this);
	}
	[[nodiscard]] constexpr inline size_t header_size() const noexcept {
		static_assert(sizeof(MessageHeader) == sizeof(uint64_t));
		return sizeof(MessageHeader);
	}

public:
	constexpr inline void type(MessageType value) noexcept {
		network_type = libv::host_to_network(libv::to_value(value));
	}
	[[nodiscard]] constexpr inline MessageType type() const noexcept {
		return MessageType{static_cast<uint8_t>(libv::network_to_host(network_type))};
	}
//	constexpr inline void reserved(uint8_t value) noexcept {
//		_reserved = libv::host_to_network(value);
//	}
//	[[nodiscard]] constexpr inline uint8_t reserved() const noexcept {
//		return libv::network_to_host(static_cast<uint8_t>(_reserved));
//	}
	constexpr inline void channel(uint16_t value) noexcept {
		network_size = libv::host_to_network(value);
	}
	[[nodiscard]] constexpr inline uint16_t channel() const noexcept {
		return libv::network_to_host(static_cast<uint16_t>(network_channel));
	}
	constexpr inline void size(uint32_t value) noexcept {
		network_size = libv::host_to_network(value);
	}
	[[nodiscard]] constexpr inline uint32_t size() const noexcept {
		return libv::network_to_host(static_cast<uint32_t>(network_size));
	}
};

// -------------------------------------------------------------------------------------------------

struct MessageEntry {
	MessageHeader header;
	MessageBody body;
};

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv
