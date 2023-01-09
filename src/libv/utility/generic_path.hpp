// Project: libv.utility, File: src/libv/utility/generic_path.hpp

#pragma once

// std
#include <filesystem>


namespace libv {

// -------------------------------------------------------------------------------------------------

// TODO P5: libv::generic_path no longer required as the GCC implementation was fixed, remove it.

[[nodiscard]] inline std::string generic_path(const std::filesystem::path& path) {
	return path.generic_string();
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
