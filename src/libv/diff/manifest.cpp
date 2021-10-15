// Project: libv.diff, File: src/libv/diff/manifest.cpp

// hpp
#include <libv/diff/manifest.hpp>
// libv
#include <libv/utility/concat.hpp>
// std
#include <algorithm>
#include <fstream>


namespace libv {
namespace diff {

// -------------------------------------------------------------------------------------------------

Manifest create_manifest(const std::filesystem::path& root_path) {
	Manifest manifest;

	for (const auto& entry : std::filesystem::recursive_directory_iterator(root_path)) {
		const auto& path = entry.path();
		const auto rel_path = std::filesystem::relative(path, root_path);
		auto path_str = rel_path.generic_string();

		if (entry.is_directory()) {
			manifest.directories.emplace_back(std::move(path_str));

		} else if (entry.is_regular_file()) {

			std::ifstream file(path, std::ios::binary | std::ios::in);

			if (!file)
				throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)), libv::concat("Failed to open file: ", path_str));

			auto md5 = libv::hash::hash_md5(file);

			file.close();

			if (file.fail())
				throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)), libv::concat("Failed to read file: ", path_str));

			manifest.files.emplace_back(std::move(path_str), std::move(md5));
		}
	}

	std::ranges::sort(manifest.directories, std::ranges::less{}, &Manifest::EntryDirectory::path);
	std::ranges::sort(manifest.files, std::ranges::less{}, &Manifest::EntryFile::path);

	return manifest;
}

// -------------------------------------------------------------------------------------------------

} // namespace diff
} // namespace libv
