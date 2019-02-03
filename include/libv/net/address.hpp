// File: address.hpp Author: Vader Created on 2017. május 8., 2:31

#pragma once

// libv
#include <libv/meta/reflection_access.hpp>
#include <libv/serialization/enable.hpp>
// std
#include <optional>
#include <string>


namespace libv {
namespace net {

// -------------------------------------------------------------------------------------------------

struct Address {
public:
	std::string address;
	std::string service;

	LIBV_REFLECTION_ACCESS(address);
	LIBV_REFLECTION_ACCESS(service);
	LIBV_SERIALIAZTION_ENABLE_REFLECTION();

public:
	inline Address() = default;
	inline Address(std::string address, uint16_t port) :
		address(std::move(address)),
		service(std::to_string(port)) { }

	inline Address(std::string address, std::string service) :
		address(std::move(address)),
		service(std::move(service)) { }

	static std::optional<Address> parse(const std::string_view addressport) {
		std::optional<Address> result;

		auto pos = addressport.find(':');
		if (pos != std::string::npos) {
			result.emplace(
					std::string{addressport.substr(0, pos)},
					std::string{addressport.substr(pos + 1, addressport.size())}
			);
		}
		return result;
	}

	template <typename OStream>
	friend OStream& operator<<(OStream& os, const Address& addr) {
		return os << addr.address << ':' << addr.service;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace net
} // namespace libv