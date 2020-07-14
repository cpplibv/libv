// File: approxing.hpp Author: Vader Created on 2017. augusztus 16., 23:59

#pragma once

// std
#include <filesystem>


namespace libv {

// -------------------------------------------------------------------------------------------------

// TODO P5: libv::generic_path no longer required as the GCC implementation was fixed, remove it.

/// I just cannot be bothered anymore, give me a generic path string
[[nodiscard]] inline std::string generic_path(const std::filesystem::path& path) {
	auto result = path.generic_string();
	std::replace(result.begin(), result.end(), '\\', '/'); // For windows drive letters
	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
