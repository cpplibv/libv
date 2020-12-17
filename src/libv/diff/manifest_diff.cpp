// Project: libv.diff, File: src/libv/diff/manifest_diff.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/diff/manifest_diff.hpp>
// libv
#include <libv/algorithm/outer_join.hpp>
// pro
#include <libv/diff/manifest.hpp>


namespace libv {
namespace diff {

// -------------------------------------------------------------------------------------------------

ManifestDiff create_manifest_diff(const Manifest& old, const Manifest& new_) {
	ManifestDiff result;

	libv::outer_join(old.directories, new_.directories, [&result](auto o, auto n) {
		if (o && n)
			return;

		const auto change = n ? ManifestDiff::ChangeDirectory::create : ManifestDiff::ChangeDirectory::remove;
		const auto& path = n ? n->path : o->path;

		result.directories.emplace_back(path, change);

	}, std::ranges::less{}, &Manifest::EntryDirectory::path);

	libv::outer_join(old.files, new_.files, [&result](auto o, auto n) {
		if (o && n && o->md5 == n->md5)
			return;

		const auto change =
				!n ? ManifestDiff::ChangeFile::remove :
				!o ? ManifestDiff::ChangeFile::create :
				ManifestDiff::ChangeFile::modify;

		const auto& path = n ? n->path : o->path;

		static constexpr auto nullopt_md5 = std::optional<libv::hash::md5>(std::nullopt);
		result.files.emplace_back(path, change, o ? o->md5 : nullopt_md5, n ? n->md5 : nullopt_md5);

	}, std::ranges::less{}, &Manifest::EntryFile::path);

	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace diff
} // namespace libv
