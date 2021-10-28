// Project: libv.update, File: app/update/apply_update.cpp

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
//#include <libv/utility/to_underlying.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/write_file.hpp>
#include <libv/update/patch.hpp>
//#include <libv/update/client/updater.hpp>
#include <libv/update/update_client/patch_applier.hpp>
//#include <libv/update/contants.hpp>


// -------------------------------------------------------------------------------------------------

int main(int argc, const char** argv) {
	auto args = libv::arg::Parser("Make Patch", "LIBV Updater patch maker");

	const auto arg_path = args.require<std::string>
			("-r", "--path")
			("path", "Path to the root directory of the ");

	const auto arg_patch_file = args.require<std::string>
			("-p", "--patch")
			("patch_file", "Output file path to the resulting patch file");

	const auto arg_verbose = args.flag
			("-v", "--verbose")
			("verbose", "Enables verbose mode");

	args.positional(arg_path, arg_patch_file);

	if (!args.standard_validate(argc, argv, std::cerr, 100))
		return EXIT_FAILURE;

	if (arg_verbose.value())
		std::cout << args.report(100);

	// ---

	const auto path_root_dir = std::filesystem::path(arg_path.value());
	const auto path_patch_file = std::filesystem::path(arg_patch_file.value());

	if (!std::filesystem::exists(path_root_dir)) {
		std::cerr << "ERROR: The specified root directory does not exists: " << path_root_dir << std::endl;
		return EXIT_FAILURE;
	}
	if (!std::filesystem::is_directory(path_root_dir)) {
		std::cerr << "ERROR: The specified root path is not directory: " << path_root_dir << std::endl;
		return EXIT_FAILURE;
	}

	if (!std::filesystem::exists(path_patch_file)) {
		std::cerr << "ERROR: The specified patch file does not exists: " << path_patch_file << std::endl;
		return EXIT_FAILURE;
	}
	if (!std::filesystem::is_regular_file(path_patch_file)) {
		std::cerr << "ERROR: The specified patch file is not a file: " << path_patch_file << std::endl;
		return EXIT_FAILURE;
	}

	// ---

	std::cout << libv::logger_stream;

	std::cout << "Loading patch... " << std::flush;
	auto patch_load_result = libv::update::Patch::load_from_file(path_patch_file);
	std::cout << "Loaded patch" << std::endl;

	libv::update::PatchApplier applier(path_root_dir, patch_load_result.patch, true);
	std::cout << "Applying patch..." << std::endl;
	while (applier.progress()) {
		const auto pt = applier.progress_total();
		const auto pc = applier.progress_current();
		std::cout << std::setw(10) << pc << "/" << pt << "   " << std::setw(3) << (static_cast<double>(pc) / static_cast<double>(pt) * 100.0) << "%" << std::endl;
	}

	if (applier.failures().empty()) {
		std::cout << "Patch applied successfully" << std::endl;
	} else {
		std::cout << "Patch applied failed:" << std::endl;
		for (const auto& failure : applier.failures())
			std::cout << "\t" << failure.filepath << " - " << failure.reason << " " << failure.reason.message() << std::endl;
	}

//	libv::update::Updater updater;
//	updater.update(path_root_dir, patch);


	return EXIT_SUCCESS;
}

// -------------------------------------------------------------------------------------------------
