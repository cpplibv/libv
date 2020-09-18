// Project: libv.utility, File: src/libv/utility/is_parent_folder_of.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <algorithm>
#include <filesystem>


namespace libv {

// -------------------------------------------------------------------------------------------------

///
/// @param parent - The parent path that should contain \c path. Expected to be in canonical form
/// @param path - The path that is expected to be within the \c parent folder. Expected to be in canonical form
/// @return
///
[[nodiscard]] inline bool is_parent_folder_of(const std::filesystem::path& parent, const std::filesystem::path& path) {
	if (std::distance(parent.begin(), parent.end()) > std::distance(path.begin(), path.end()))
		return false;

	const auto [parent_it, path_it] = std::mismatch(parent.begin(), parent.end(), path.begin(), path.end());
	return parent_it == parent.end();
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
