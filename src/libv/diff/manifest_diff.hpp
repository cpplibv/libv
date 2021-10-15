// Project: libv.diff, File: src/libv/diff/manifest_diff.hpp

#pragma once

// libv
#include <libv/hash/md5.hpp>
// std
#include <optional>
#include <string>
#include <vector>


namespace libv {
namespace diff {

// -------------------------------------------------------------------------------------------------

struct Manifest;

struct ManifestDiff {
public:
	enum class ChangeFile : uint32_t {
		create = 0,
		modify = 1,
		// ModifyAs = 2,
		// rename = 3,
		remove = 4,
	};

	enum class ChangeDirectory : uint32_t {
		create = 0,
		remove = 4,
	};

public:
	struct EntryDirectory {
		std::string filepath;
		ChangeDirectory change;
	};

	struct EntryFile {
		std::string filepath;
		ChangeFile change;
		std::optional<libv::hash::md5> old_md5;
		std::optional<libv::hash::md5> new_md5;
	};

public:
	std::vector<EntryDirectory> directories;
	std::vector<EntryFile> files;
};

[[nodiscard]] ManifestDiff create_manifest_diff(const Manifest& old, const Manifest& new_);

// -------------------------------------------------------------------------------------------------

} // namespace diff
} // namespace libv
