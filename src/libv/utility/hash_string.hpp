// Project: libv.utility, File: src/libv/utility/hash_string.hpp

#pragma once

// libv
#include <libv/meta/force_inline.hpp>
// std
#include <functional>
#include <string>
#include <string_view>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// Usage: std::unordered_multimap<std::string, Feature, libv::hash_string, std::equal_to<>> features;
///
struct hash_string {
	using is_transparent = void;
	using hash_type = std::hash<std::string_view>;

public:
	[[nodiscard]] LIBV_FORCE_INLINE std::size_t operator()(const char* txt) const noexcept {
		return hash_type{}(txt);
	}
	[[nodiscard]] LIBV_FORCE_INLINE std::size_t operator()(std::string_view txt) const noexcept {
		return hash_type{}(txt);
	}
	[[nodiscard]] LIBV_FORCE_INLINE std::size_t operator()(const std::string& txt) const noexcept {
		return hash_type{}(txt);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
