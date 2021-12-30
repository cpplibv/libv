// Project: libv.hash, File: src/libv/hash/md5.hpp

#pragma once

// std
#include <cstring>
#include <filesystem>
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
	template <typename Archive>
	inline void serialize(Archive& ar) {
		ar(value);
	}
};

// -------------------------------------------------------------------------------------------------

[[nodiscard]] md5 hash_md5(const std::string_view data);
[[nodiscard]] md5 hash_md5(const std::span<const std::byte> data);
[[nodiscard]] md5 hash_md5(std::istream& stream);

// -------------------------------------------------------------------------------------------------

[[nodiscard]] md5 hash_file_md5_or_throw(const std::filesystem::path& filepath);
[[nodiscard]] md5 hash_file_md5(const std::filesystem::path& filepath, std::error_code& ec);
[[nodiscard]] inline auto hash_file_md5_ec(const std::filesystem::path& filepath) {
	struct Result {
		std::error_code ec;
		md5 hash;
	} result;

	result.hash = hash_file_md5(filepath, result.ec);

	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace hash
} // namespace libv
