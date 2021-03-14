// Project: libv.net, File: src/libv/net/address.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/meta/reflection_access.hpp>
#include <libv/serial/enable.hpp>
#include <libv/serial/limit.hpp>
// std
#include <optional>
#include <string>
#include <string_view>


namespace libv {
namespace net {

// -------------------------------------------------------------------------------------------------

struct Address {
public:
	libv::serial::limit<std::string, 256> address;
	libv::serial::limit<std::string, 256> service;

	LIBV_REFLECTION_ACCESS(address);
	LIBV_REFLECTION_ACCESS(service);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	inline Address() = default;

	template <typename Str>
			requires std::convertible_to<Str, std::string>
	inline Address(Str&& address, uint16_t port) :
		address(std::forward<Str>(address)),
		service(std::to_string(port)) { }

	template <typename Str0, typename Str1>
			requires (std::convertible_to<Str0, std::string> && std::convertible_to<Str1, std::string>)
	inline Address(Str0&& address, Str1&& port) :
		address(std::forward<Str0>(address)),
		service(std::forward<Str1>(port)) { }

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
	inline friend OStream& operator<<(OStream& os, const Address& addr) {
		os << addr.address << ':' << addr.service;
		return os;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace net
} // namespace libv
