// Project: libv.net, File: src/libv/net/mtcp/endpoint.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/net/mtcp/endpoint.hpp>
// libv
#include <libv/utility/parse_number.hpp>
// std
#include <cstdint>


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

Endpoint parse_endpoint_or_throw(std::string_view address, uint16_t port) {
	size_t p0 = address.find('.', 0);
	if (p0 == std::string_view::npos)
		throw std::invalid_argument("\"" + std::string(address) + "\" is not a valid IPv4 address, missing first '.'");

	size_t p1 = address.find('.', p0 + 1);
	if (p1 == std::string_view::npos)
		throw std::invalid_argument("\"" + std::string(address) + "\" is not a valid IPv4 address, missing second '.'");

	size_t p2 = address.find('.', p1 + 1);
	if (p2 == std::string_view::npos)
		throw std::invalid_argument("\"" + std::string(address) + "\" is not a valid IPv4 address, missing third '.'");

	try {
		const auto a0 = libv::parse_number_or_throw<uint8_t>(address.substr(0, p0));
		const auto a1 = libv::parse_number_or_throw<uint8_t>(address.substr(p0 + 1, p1 - p0 - 1));
		const auto a2 = libv::parse_number_or_throw<uint8_t>(address.substr(p1 + 1, p2 - p1 - 1));
		const auto a3 = libv::parse_number_or_throw<uint8_t>(address.substr(p2 + 1, address.size() - p2));
		return Endpoint(a0, a1, a2, a3, port);
	} catch (const std::exception& e) {
		throw std::invalid_argument("\"" + std::string(address) + "\" is not a valid IPv4 address: " + e.what());
	}
}

Endpoint parse_endpoint_or_throw(std::string_view endpoint) {
	size_t p0 = endpoint.find(':', 0);
	if (p0 == std::string_view::npos)
		throw std::invalid_argument("\"" + std::string(endpoint) + "\" is not a valid IPv4 endpoint, missing ':'");

	std::string_view address;
	uint16_t port;

	try {
		address = endpoint.substr(0, p0);
		port = libv::parse_number_or_throw<uint16_t>(endpoint.substr(p0 + 1, endpoint.size() - p0));
	} catch (const std::exception& e) {
		throw std::invalid_argument("\"" + std::string(endpoint) + "\" is not a valid IPv4 endpoint: " + e.what());
	}

	return parse_endpoint_or_throw(address, port);
}

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv
