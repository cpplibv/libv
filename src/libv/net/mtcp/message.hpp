// Project: libv.net, File: src/libv/net/mtcp/message.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/utility/byte_swap.hpp>
#include <libv/utility/storage_size.hpp>
#include <libv/utility/to_underlying.hpp>
// std
#include <cstdint>
#include <span>
#include <string>
#include <vector>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

using message_body_bin = std::vector<std::byte>;
using message_body_bin_view = std::span<const std::byte>;
using message_body_str = std::string;
using message_body_str_view = std::string_view;

using message_body = message_body_str_view;
using message_body_view = message_body_bin_view;

// -------------------------------------------------------------------------------------------------

constexpr auto MTCP_MESSAGE_MAX_RESERVE = libv::MB(1);
constexpr auto MTCP_MESSAGE_MAX_SIZE = libv::MB(4);

// -------------------------------------------------------------------------------------------------

enum class mtcp_message_type : uint8_t {
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

struct message_header {
private:
	uint64_t network_type : 8;
	uint64_t _reserved : 8;
	uint64_t network_channel : 16;
	uint64_t network_size : 32;

public:
	explicit constexpr inline message_header(
			mtcp_message_type type = mtcp_message_type::message,
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
		static_assert(sizeof(message_header) == sizeof(uint64_t));
		return sizeof(message_header);
	}

public:
	constexpr inline void type(mtcp_message_type value) noexcept {
		network_type = libv::host_to_network(libv::to_value(value));
	}
	[[nodiscard]] constexpr inline mtcp_message_type type() const noexcept {
		return mtcp_message_type{static_cast<uint8_t>(libv::network_to_host(network_type))};
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

static_assert(MTCP_MESSAGE_MAX_SIZE < std::numeric_limits<decltype(std::declval<message_header>().size())>::max());
static_assert(sizeof(message_header) == 8); // Just a check, not a must

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv
