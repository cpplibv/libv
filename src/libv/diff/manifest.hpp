// Project: libv.diff, File: src/libv/diff/manifest.hpp

#pragma once

// libv
#include <libv/hash/md5.hpp>
// std
#include <filesystem>
#include <optional>
#include <string>
#include <vector>


namespace libv {
namespace diff {

// -------------------------------------------------------------------------------------------------

struct Manifest {
public:
	struct EntryDirectory {
		std::string path;
	};
	struct EntryFile {
		std::string path;
		libv::hash::md5 md5;
	};

public:
	std::vector<EntryDirectory> directories;
	std::vector<EntryFile> files;
};

[[nodiscard]] Manifest create_manifest(const std::filesystem::path& root_path);

// -------------------------------------------------------------------------------------------------

} // namespace diff
} // namespace libv
