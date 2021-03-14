// Project: libv.update, File: app/update/create_update.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/arg/arg.hpp>
#include <libv/diff/diff.hpp>
#include <libv/diff/manifest.hpp>
#include <libv/log/log.hpp>
// std
#include <filesystem>
#include <iostream>
// pro



#include <fstream>
#include <libv/hash/md5.hpp>
//#include <libv/utility/enum.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/write_file.hpp>
#include <libv/update/patch.hpp>
//#include <libv/update/contants.hpp>


// -------------------------------------------------------------------------------------------------

int main(int argc, const char** argv) {
	auto args = libv::arg::Parser("Make Patch", "LIBV Updater patch maker");

	const auto arg_old_dir = args.require<std::string>
			("-o", "--old")
			("old_dir", "Path to the root directory of the old version");

	const auto arg_new_dir = args.require<std::string>
			("-n", "--new")
			("new_dir", "Path to the root directory of the new version");

	const auto arg_patch_file = args.require<std::string>
			("-p", "--patch")
			("patch_file", "Output file path to the resulting patch file");

	const auto arg_force = args.flag
			("-f", "--force")
			("force", "Override the patch file if it already exists");

	const auto arg_cache = args.flag
			("-c", "--cache")
			("cache", "Allows cached manifest creation and usage");

	const auto arg_verbose = args.flag
			("-v", "--verbose")
			("verbose", "Enables verbose mode");

//	const auto arg_invalidate = args.flag
//			("-i", "--invalidate")
//			("invalidate", "Invalidate any cached manifest and regenerate them is caching is enabled");

	args.positional(arg_old_dir, arg_new_dir, arg_patch_file);

	if (!args.standard_validate(argc, argv, std::cerr, 100))
		return EXIT_FAILURE;

	if (arg_verbose.value())
		std::cout << args.report(100);

	// ---

	const auto path_old_dir = std::filesystem::path(arg_old_dir.value());
	const auto path_new_dir = std::filesystem::path(arg_new_dir.value());
	const auto path_patch_file = std::filesystem::path(arg_patch_file.value());

	if (!std::filesystem::exists(path_old_dir)) {
		std::cerr << "ERROR: The specified old version root directory does not exists: " << path_old_dir << std::endl;
		return EXIT_FAILURE;
	}
	if (!std::filesystem::is_directory(path_old_dir)) {
		std::cerr << "ERROR: The specified old version root path is not directory: " << path_old_dir << std::endl;
		return EXIT_FAILURE;
	}

	if (!std::filesystem::exists(path_new_dir)) {
		std::cerr << "ERROR: The specified new version root directory does not exists: " << path_new_dir << std::endl;
		return EXIT_FAILURE;
	}
	if (!std::filesystem::is_directory(path_new_dir)) {
		std::cerr << "ERROR: The specified new version root path is not directory: " << path_new_dir << std::endl;
		return EXIT_FAILURE;
	}

	if (std::filesystem::exists(path_patch_file)) {
		if (std::filesystem::is_directory(path_patch_file)) {
			std::cerr << "ERROR: The specified patch file is already a directory: " << path_patch_file << std::endl;
			return EXIT_FAILURE;
		}
		if (!arg_force.value()) {
			std::cerr << "ERROR: The specified patch file already exists: " << path_patch_file << std::endl;
			return EXIT_FAILURE;
		}
	}

	// ---

	std::cout << libv::logger_stream;

//	const auto filename_manifest = ".manifest";

//// const auto load_manifest = [&](const auto& path_dir) {
//// 	if (arg_cache.value()) {
//// 		const auto path_manifest_file = path_dir / filename_manifest;
//
//// 		if (std::filesystem::exists(path_manifest_file)) {
//// 			const auto manifest_content = libv::read_file_or_throw(path_manifest_file);
//// 			return libv::diff::parse_manifest(manifest_content);
//// 		} else {
//// 			const auto manifest = libv::diff::create_manifest(path_dir);
//// 			libv::write_file_or_throw(manifest.to_data());
//// 			return manifest;
//// 		}
//// 	}
//
//// 	return libv::diff::create_manifest(path_dir);
//// };
//
//	const auto load_manifest = [&](const auto& path_dir) {
//	//	const auto filepath_manifest = path_old_dir / filename_manifest;
//	//	if (std::filesystem::exists(filepath_manifest))
//		if (manifest_exists && arg_cache.value() && !arg_invalidate.value()) {
//			return libv::diff::ManifestDiff::load_from_file(filepath_manifest);
//
//		} else if (!arg_cache.value()) {
//			return libv::diff::create_manifest(path_old_dir);
//
//		} else {
//			const auto manifest = libv::diff::create_manifest(path_old_dir);
//			manifest.save_to_file(filepath_manifest);
//			return manifest;
//		}
//	};

	std::cout << "Creating manifest for: " << path_old_dir.generic_string() << "... " << std::flush;
	const auto manifest_old = libv::diff::create_manifest(path_old_dir);
	std::cout << manifest_old.files.size() << " Entry" << std::endl;

	std::cout << "Creating manifest for: " << path_new_dir.generic_string() << "... " << std::flush;
	const auto manifest_new = libv::diff::create_manifest(path_new_dir);
	std::cout << manifest_new.files.size() << " Entry" << std::endl;

	std::cout << "Creating manifest diff... " << std::flush;
	const auto manifest_diff = libv::diff::create_manifest_diff(manifest_old, manifest_new);
	std::cout << manifest_diff.files.size() << " Entry" << std::endl;

	std::cout << "Creating patch... " << std::flush;
	const auto patch = libv::update::Patch::create(manifest_diff, path_old_dir, path_new_dir);

	std::cout << "Create: " << patch.creates.size()
			<< ", Modify: " << patch.modifies.size()
			<< ", Remove: " << patch.removes.size() << std::endl;

	std::cout << "Saving patch " << path_patch_file.generic_string() << "..." << std::endl;
	patch.save_to_file(path_patch_file);

	std::ifstream file(path_patch_file, std::ios::binary | std::ios::in);
	std::cout << "Patch MD5: " << libv::hash::hash_md5(file) << ", Size: " << libv::read_file_str_or_throw(path_patch_file).size() << " byte" << std::endl;

	return EXIT_SUCCESS;
}

// -------------------------------------------------------------------------------------------------
