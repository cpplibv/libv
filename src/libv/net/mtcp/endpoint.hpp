// Project: libv.net, File: src/libv/net/mtcp/endpoint.hpp

#pragma once

// libv
#include <libv/meta/reflection_access.hpp>
#include <libv/serial/enable.hpp>
// std
#include <array>
#include <cstdint>
#include <string_view> // Only for parse functions


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

struct Endpoint {
	std::array<uint8_t, 4> address;
	uint16_t port;

	LIBV_REFLECTION_ACCESS(address);
	LIBV_REFLECTION_ACCESS(port);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	constexpr inline Endpoint() = default;

	explicit constexpr inline Endpoint(uint16_t port) noexcept :
		address{0, 0, 0, 0},
		port(port) { }

	constexpr inline Endpoint(std::array<uint8_t, 4> address, uint16_t port) noexcept :
		address(address),
		port(port) { }

	constexpr inline Endpoint(uint8_t a0, uint8_t a1, uint8_t a2, uint8_t a3, uint16_t port) noexcept :
		address{a0, a1, a2, a3},
		port(port) { }

public:
	constexpr inline void clear() noexcept {
		address.fill(0);
		port = 0;
	}

	template <typename OStream>
	friend inline OStream& operator<<(OStream& os, const Endpoint& addr) {
		os
				<< uint16_t{addr.address[0]} << '.'
				<< uint16_t{addr.address[1]} << '.'
				<< uint16_t{addr.address[2]} << '.'
				<< uint16_t{addr.address[3]} << ':'
				<< addr.port;
		return os;
	}
};

// -------------------------------------------------------------------------------------------------

Endpoint parse_endpoint_or_throw(std::string_view address, uint16_t port);
Endpoint parse_endpoint_or_throw(std::string_view endpoint);

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv
