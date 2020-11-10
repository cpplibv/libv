// Project: libv.diff, File: src/libv/diff/dir.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/diff/manifest.hpp>
// libv
#include <libv/algorithm/outer_join.hpp>
// std
#include <algorithm>
#include <filesystem>
#include <fstream>


namespace libv {
namespace diff {

// -------------------------------------------------------------------------------------------------

Manifest create_manifest(const std::string_view root_path_str) {
	Manifest manifest;

	std::filesystem::path root_path = root_path_str;

	for (const auto& entry : std::filesystem::recursive_directory_iterator(root_path)) {
		if (not entry.is_regular_file())
			continue;

		const auto& path = entry.path();
		const auto rel_path = std::filesystem::relative(path, root_path);

		std::ifstream file(path, std::ios::binary | std::ios::in);
		auto md5 = libv::hash::hash_md5(file);
		auto path_str = rel_path.generic_string();

		manifest.entries.emplace_back(std::move(path_str), std::move(md5));
	}

	std::ranges::sort(manifest.entries, std::ranges::less{}, &Manifest::Entry::path);

	return manifest;
}

ManifestDiff create_manifest_diff(const Manifest& old, const Manifest& new_) {
	ManifestDiff result;

	libv::outer_join(old.entries, new_.entries, [&result](auto o, auto n) {
		if (o && n && o->md5 == n->md5)
			return;

		const auto change =
				!n ? ManifestDiff::Change::remove :
				!o ? ManifestDiff::Change::add :
				ManifestDiff::Change::modify;

		const auto& path = n ? n->path : o->path;

		result.entries.emplace_back(path, change);

	}, std::ranges::less{}, &Manifest::Entry::path);

	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace diff
} // namespace libv
