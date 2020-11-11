// Project: libv.diff, File: src/libv/diff/dir.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/hash/md5.hpp>
// std
#include <filesystem>
#include <string>
#include <vector>


namespace libv {
namespace diff {

// -------------------------------------------------------------------------------------------------

struct Manifest {
public:
	struct Entry {
		std::string path;
		libv::hash::md5 md5;
	};

public:
	std::vector<Entry> entries;
};

struct ManifestDiff {
public:
	enum class Change {
		add,
		remove,
		modify,
		// rename, ?
		// replace, ?
	};

	struct Entry {
		std::string path;
		Change change;
	};

public:
	std::vector<Entry> entries;
};

// -------------------------------------------------------------------------------------------------

[[nodiscard]] Manifest create_manifest(const std::filesystem::path& root_path);
[[nodiscard]] ManifestDiff create_manifest_diff(const Manifest& old, const Manifest& new_);

// -------------------------------------------------------------------------------------------------

} // namespace diff
} // namespace libv
