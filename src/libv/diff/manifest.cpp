// Project: libv.diff, File: src/libv/diff/dir.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/diff/manifest.hpp>
// libv
#include <libv/algorithm/outer_join.hpp>
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
		if (not entry.is_regular_file())
			continue;

		const auto& path = entry.path();
		const auto rel_path = std::filesystem::relative(path, root_path);
		auto path_str = rel_path.generic_string();

		std::ifstream file(path, std::ios::binary | std::ios::in);

		if (!file)
			throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)), libv::concat("Failed to open file: ", path_str));

		auto md5 = libv::hash::hash_md5(file);

		file.close();

		if (file.fail())
			throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)), libv::concat("Failed to read file: ", path_str));

		manifest.entries.emplace_back(std::move(path_str), std::move(md5));
	}

	std::ranges::sort(manifest.entries, std::ranges::less{}, &Manifest::Entry::path);

	return manifest;
}

ManifestDiff create_manifest_diff(const Manifest& old, const Manifest& new_) {
	ManifestDiff manifest_diff;

	libv::outer_join(old.entries, new_.entries, [&manifest_diff](auto o, auto n) {
		if (o && n && o->md5 == n->md5)
			return;

		const auto change =
				!n ? ManifestDiff::Change::remove :
				!o ? ManifestDiff::Change::add :
				ManifestDiff::Change::modify;

		const auto& path = n ? n->path : o->path;

		manifest_diff.entries.emplace_back(path, change);

	}, std::ranges::less{}, &Manifest::Entry::path);

	return manifest_diff;
}

// -------------------------------------------------------------------------------------------------

} // namespace diff
} // namespace libv
