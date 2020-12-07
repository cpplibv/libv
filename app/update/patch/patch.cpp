// Project: libv, File: app/update/patch/patch.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <update/patch/patch.hpp>
//// ext
//#include <boost/interprocess/exceptions.hpp>
// libv
#include <libv/arg/arg.hpp>
#include <libv/diff/diff.hpp>
#include <libv/diff/manifest.hpp>
#include <libv/hash/md5.hpp>
#include <libv/log/log.hpp>
// std
#include <filesystem>
#include <iostream>
#include <mutex>
// pro
//#include <update/patch/constants.hpp>


//#include <libv/mt/lock_file.hpp>

//#include <libv/utility/enum.hpp>
#include <libv/utility/read_file.hpp>
//#include <libv/utility/write_file.hpp>
//#include <libv/utility/byte_swap.hpp>

////// ext
////#include <boost/interprocess/exceptions.hpp>
//// libv
//#include <libv/arg/arg.hpp>
//#include <libv/diff/diff.hpp>
//#include <libv/diff/manifest.hpp>
//#include <libv/log/log.hpp>
//#include <libv/utility/function_ref.hpp>
//// std
//#include <filesystem>
//#include <fstream>
//// pro
////#include <update/common/contants.hpp>
//
//
//
//
//#include <mutex>
//#include <iostream>
//#include <variant>
//#include <libv/process/lock_file.hpp>
////#include <fstream>
//#include <libv/utility/enum.hpp>
//#include <libv/utility/read_file.hpp>
//#include <libv/utility/write_file.hpp>
//#include <libv/utility/byte_swap.hpp>
//#include <update/common/log.hpp>






#include <libv/serialization/reflection.hpp>
#include <libv/serialization/types/std_vector.hpp>
#include <libv/serialization/types/std_string.hpp>
#include <libv/serialization/archive/binary.hpp>
#include <fstream>



namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

std::error_code Patch::save_to_file(const std::filesystem::path& filepath) const {
	if (!filepath.parent_path().empty())
		std::filesystem::create_directories(filepath.parent_path());
	std::ofstream file(filepath, std::ios::binary | std::ios::out);

	{
		libv::archive::BinaryOutput ar{file};
		ar(*this);
	}

	return {};
}

Patch::LoadResult Patch::load_from_file(const std::filesystem::path& filepath) {
	auto patch = std::make_shared<Patch>();

	std::ifstream file(filepath, std::ios::binary | std::ios::out);

	{
		libv::archive::BinaryInput ar{file};
		ar(*patch);
	}

	LoadResult result;
	result.patch = std::move(patch);
	return result;
}

Patch Patch::create(const libv::diff::ManifestDiff& manifest_diff, const std::filesystem::path& root_old, const std::filesystem::path& root_new) {
	Patch patch;

	patch.version = current_patch_version;
//	patch.number = ?;

//	patch.from = ?;
//	patch.to = ?;

	for (const auto& entry : manifest_diff.directories) {
		switch (entry.change) {
		case libv::diff::ManifestDiff::ChangeDirectory::create:
			patch.creates_dir.emplace_back(entry.filepath);
			break;

		case libv::diff::ManifestDiff::ChangeDirectory::remove:
			patch.removes_dir.emplace_back(entry.filepath);
			break;
		}
	}

	for (const auto& entry : manifest_diff.files) {
		switch (entry.change) {
		case libv::diff::ManifestDiff::ChangeFile::create:
			patch.creates.emplace_back(
					entry.filepath,
					libv::read_file_bin_or_throw(root_new / entry.filepath),
					*entry.new_md5);
			break;

		case libv::diff::ManifestDiff::ChangeFile::remove:
			patch.removes.emplace_back(
					entry.filepath,
					*entry.old_md5);
			break;

		case libv::diff::ManifestDiff::ChangeFile::modify:
			std::ifstream old_file(root_old / entry.filepath, std::ios::binary | std::ios::in);
			std::ifstream new_file(root_new / entry.filepath, std::ios::binary | std::ios::in);

			patch.modifies.emplace_back(
					entry.filepath,
					libv::diff::create_diff_bin(old_file, new_file),
					*entry.old_md5,
					*entry.new_md5);
			break;
		}
	}

	return patch;
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
