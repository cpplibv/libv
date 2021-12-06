// Project: libv.utility, File: src/libv/utility/hash_string.hpp

#pragma once

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
	[[nodiscard]] inline std::size_t operator()(const char* txt) const {
		return hash_type{}(txt);
	}
	[[nodiscard]] inline std::size_t operator()(std::string_view txt) const {
		return hash_type{}(txt);
	}
	[[nodiscard]] inline std::size_t operator()(const std::string& txt) const {
		return hash_type{}(txt);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
