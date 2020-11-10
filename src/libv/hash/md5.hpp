// Project: libv.hash, File: src/libv/hash/md5.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <cstring>
#include <iosfwd>
#include <span>
#include <string_view>


namespace libv {
namespace hash {

// -------------------------------------------------------------------------------------------------

struct md5 {
	std::byte value[16];

public:
	[[nodiscard]] friend constexpr inline bool operator==(const md5& lhs, const md5& rhs) noexcept {
		return std::memcmp(lhs.value, rhs.value, 16) == 0;
	}

	friend std::ostream& operator<<(std::ostream& os, const md5& md5);

public:
	template <class Archive>
	inline void serialize(Archive& ar) {
		ar(value);
	}
};

[[nodiscard]] md5 hash_md5(const std::string_view data);
[[nodiscard]] md5 hash_md5(const std::span<const std::byte> data);
[[nodiscard]] md5 hash_md5(std::istream& stream);

// -------------------------------------------------------------------------------------------------

} // namespace hash
} // namespace libv
