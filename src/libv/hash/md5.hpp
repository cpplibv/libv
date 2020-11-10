// Project: libv.hash, File: src/libv/hash/md5.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <iosfwd>
#include <span>
#include <string>
#include <string_view>


namespace libv {
namespace hash {

// -------------------------------------------------------------------------------------------------

struct md5 {
	std::byte value[16];

public:
	[[nodiscard]] friend constexpr inline bool operator==(const md5& lhs, const md5& rhs) noexcept {
		for (int i = 0; i < 16; i++)
			if (lhs.value[i] != rhs.value[i])
				return false;
		return true;
	}

	friend std::ostream& operator<<(std::ostream& os, const md5& md5);
};

[[nodiscard]] md5 hash_md5(const std::string_view data);
[[nodiscard]] md5 hash_md5(const std::span<const std::byte> data);
[[nodiscard]] md5 hash_md5(std::istream& stream);

// -------------------------------------------------------------------------------------------------

} // namespace hash
} // namespace libv
