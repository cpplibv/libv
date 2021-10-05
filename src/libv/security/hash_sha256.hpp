// Project: libv.security, File: src/libv/security/hash_sha256.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <span>
#include <string_view>


namespace libv {
namespace security {

// -------------------------------------------------------------------------------------------------

struct SHA256 {
	std::byte value[32] = {};
};

[[nodiscard]] SHA256 hash_sha256(std::span<const std::byte> data);
[[nodiscard]] SHA256 hash_sha256(std::string_view data);

// -------------------------------------------------------------------------------------------------

} // namespace security
} // namespace libv
