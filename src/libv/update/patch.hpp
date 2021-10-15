// Project: libv.update, File: src/libv/update/patch.hpp

#pragma once

// libv
//#include <libv/diff/manifest.hpp>
#include <libv/diff/manifest_diff.hpp>
#include <libv/hash/md5.hpp>
#include <libv/meta/reflection_access.hpp>
#include <libv/serial/enable.hpp>
// std
#include <filesystem>
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>
// pro
//#include <libv/update/patch_number.hpp>
#include <libv/update/patch_version.hpp>
#include <libv/update/version_number.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

using file_diff_data = std::vector<std::byte>;
using file_full_data = std::vector<std::byte>;

// -------------------------------------------------------------------------------------------------

struct StepCreateDir {
	std::string path;

	LIBV_REFLECTION_ACCESS(path);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

struct StepRemoveDir {
	std::string path;

	LIBV_REFLECTION_ACCESS(path);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

struct StepCreate {
	std::string filepath;
	file_full_data data;
	libv::hash::md5 hash_new;

	LIBV_REFLECTION_ACCESS(filepath);
	LIBV_REFLECTION_ACCESS(data);
	LIBV_REFLECTION_ACCESS(hash_new);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

struct StepModify {
	std::string filepath;
	file_diff_data diff;
	libv::hash::md5 hash_old;
	libv::hash::md5 hash_new;

	LIBV_REFLECTION_ACCESS(filepath);
	LIBV_REFLECTION_ACCESS(diff);
	LIBV_REFLECTION_ACCESS(hash_old);
	LIBV_REFLECTION_ACCESS(hash_new);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

struct StepModifyAs {
	std::string filepath_old;
	std::string filepath_new;
	file_diff_data diff;
	libv::hash::md5 hash_old;
	libv::hash::md5 hash_new;

	LIBV_REFLECTION_ACCESS(filepath_old);
	LIBV_REFLECTION_ACCESS(filepath_new);
	LIBV_REFLECTION_ACCESS(diff);
	LIBV_REFLECTION_ACCESS(hash_old);
	LIBV_REFLECTION_ACCESS(hash_new);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

struct StepRename {
	std::string filepath_old;
	std::string filepath_new;
	libv::hash::md5 hash_new;

	LIBV_REFLECTION_ACCESS(filepath_old);
	LIBV_REFLECTION_ACCESS(filepath_new);
	LIBV_REFLECTION_ACCESS(hash_new);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

struct StepRemove {
	std::string filepath;
	libv::hash::md5 hash_old;

	LIBV_REFLECTION_ACCESS(filepath);
	LIBV_REFLECTION_ACCESS(hash_old);
	LIBV_SERIALIZATION_ENABLE_REFLECTION();
};

// -------------------------------------------------------------------------------------------------

struct Patch {
	static constexpr patch_version current_patch_version = patch_version{0};

	patch_version version;

	version_number source;
	version_number target;

public:
	std::vector<StepCreateDir> creates_dir;
	std::vector<StepRemoveDir> removes_dir;

	std::vector<StepCreate> creates;
	std::vector<StepModify> modifies;
	std::vector<StepModifyAs> modifies_as;
	std::vector<StepRename> renames;
	std::vector<StepRemove> removes;

public:
	LIBV_REFLECTION_ACCESS(version);

	LIBV_REFLECTION_ACCESS(source);
	LIBV_REFLECTION_ACCESS(target);

	LIBV_REFLECTION_ACCESS(creates_dir);
	LIBV_REFLECTION_ACCESS(removes_dir);

	LIBV_REFLECTION_ACCESS(creates);
	LIBV_REFLECTION_ACCESS(modifies);
	LIBV_REFLECTION_ACCESS(modifies_as);
	LIBV_REFLECTION_ACCESS(renames);
	LIBV_REFLECTION_ACCESS(removes);

	LIBV_SERIALIZATION_ENABLE_REFLECTION();

public:
	struct [[nodiscard]] LoadResult {
		std::error_code ec;
		std::shared_ptr<const Patch> patch;
	};

	[[nodiscard]] std::error_code save_to_file(const std::filesystem::path& filepath) const;
	[[nodiscard]] std::error_code save_to(std::ostream& ostream);
	[[nodiscard]] static LoadResult load_from_file(const std::filesystem::path& filepath);
	[[nodiscard]] static LoadResult load_from(const std::istream& istream);

	static Patch create(const libv::diff::ManifestDiff& manifest_diff, const std::filesystem::path& root_old, const std::filesystem::path& root_new);
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
