// Project: libv.net, File: src/libv/net/mtcp/endpoint.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/meta/reflection_access.hpp>
#include <libv/serialization/enable.hpp>
// std
#include <array>
#include <cstddef>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

struct Endpoint {
	std::array<uint8_t, 4> address;
	uint16_t port;

	LIBV_REFLECTION_ACCESS(address);
	LIBV_REFLECTION_ACCESS(port);
	LIBV_SERIALIAZTION_ENABLE_REFLECTION();

	void clear() {
		address.fill(0);
		port = 0;
	}

	template <typename OStream>
	friend OStream& operator<<(OStream& os, const Endpoint& addr) {
		return os
				<< uint16_t{addr.address[0]} << '.'
				<< uint16_t{addr.address[1]} << '.'
				<< uint16_t{addr.address[2]} << '.'
				<< uint16_t{addr.address[3]} << ':'
				<< addr.port;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv
