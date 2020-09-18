// Project: libv.utility, File: src/libv/utility/generic_path.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <filesystem>


namespace libv {

// -------------------------------------------------------------------------------------------------

// TODO P5: libv::generic_path no longer required as the GCC implementation was fixed, remove it.

/// I just cannot be bothered anymore, give me a generic path string
[[nodiscard]] inline std::string generic_path(const std::filesystem::path& path) {
	auto result = path.generic_string();
	std::replace(result.begin(), result.end(), '\\', '/'); // For windows drive letters only
	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
