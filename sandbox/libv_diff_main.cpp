// Project: libv.diff, File: sandbox/libv_diff_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/utility/hex_dump.hpp>
// std
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
// pro
#include <libv/diff/diff.hpp>


// -------------------------------------------------------------------------------------------------

std::string_view as_sv(const std::span<const std::byte> s) {
	return std::string_view(reinterpret_cast<const char*>(s.data()), s.size());
}

std::string_view as_sv(const std::string_view s) {
	return s;
}

std::vector<std::byte> to_bytes(std::string_view str) {
	std::vector<std::byte> result;

	result.resize(str.size());
	std::memcpy(result.data(), str.data(), result.size());

	return result;
}

// -------------------------------------------------------------------------------------------------

template <typename T>
bool test_memory(const T& old_, const T& new_, size_t block_size) {
	std::cout << "=================================================================================================" << std::endl;

	const auto diff = libv::diff::create_diff(old_, new_, block_size);

	std::cout << "Old_ size: " << old_.size() << std::endl;
	std::cout << "New_ size: " << new_.size() << std::endl;
	std::cout << "Diff size: " << diff.size() << std::endl;

	std::cout << "Diff hex:" << std::endl;
	std::cout << libv::hex_dump_with_ascii(diff) << std::endl;

	const auto diff_info = libv::diff::get_diff_info(diff);
	std::cout << "diff_info: " << diff_info.valid() << std::endl;

	if (!diff_info)
		return false;

	std::cout << "diff_info.old_size: " << diff_info.old_size << std::endl;
	std::cout << "diff_info.new_size: " << diff_info.new_size << std::endl;

	const auto check_result = libv::diff::check_diff(old_, new_, diff);

	std::cout << "check_result: " << check_result << std::endl;

	const auto patch_result = libv::diff::apply_patch(old_, diff);

	std::cout << "patch_result: " << patch_result.has_value() << std::endl;

	if (!patch_result)
		return false;

	std::cout << "patch_result: " << as_sv(*patch_result) << std::endl;
	std::cout << "new == patch_result: " << (new_ == *patch_result) << std::endl;

	return true;
}

bool test_stream(const std::string_view str_old_, const std::string_view str_new_, size_t block_size) {
	std::cout << "=================================================================================================" << std::endl;

	std::istringstream old_{std::string(str_old_)};
	std::istringstream new_{std::string(str_new_)};
	std::stringstream diff;

	libv::diff::create_diff(old_, new_, diff, block_size);

	std::cout << "Old_ size: " << old_.str().size() << std::endl;
	std::cout << "New_ size: " << new_.str().size() << std::endl;
	std::cout << "Diff size: " << diff.str().size() << std::endl;

	std::cout << "Diff hex:" << std::endl;
	std::cout << libv::hex_dump_with_ascii(diff.str()) << std::endl;

	const auto diff_info = libv::diff::get_diff_info(diff);
	std::cout << "diff_info: " << diff_info.valid() << std::endl;

	if (!diff_info)
		return false;

	std::cout << "diff_info.old_size: " << diff_info.old_size << std::endl;
	std::cout << "diff_info.new_size: " << diff_info.new_size << std::endl;

	const auto check_result = libv::diff::check_diff(old_, new_, diff);

	std::cout << "check_result: " << check_result << std::endl;

	std::stringstream patched;
	const auto patch_result = libv::diff::apply_patch(old_, diff, patched);

	std::cout << "patch_result: " << patch_result << std::endl;

	if (!patch_result)
		return false;

	std::cout << "patched: " << patched.str() << std::endl;
	std::cout << "new == patched: " << (new_.str() == patched.str()) << std::endl;

	return true;
}

bool test_file(const std::string_view file_old_, const std::string_view file_new_, size_t block_size) {
	std::cout << "=================================================================================================" << std::endl;

	const auto diff_file_path = std::string(file_old_) + ".diff";
	const auto patched_file_path = std::string(file_old_) + ".patched";

	std::ifstream old_{std::string(file_old_), std::ios::in | std::ios::binary};
	std::ifstream new_{std::string(file_new_), std::ios::in | std::ios::binary};
	std::fstream diff{diff_file_path, std::ios::out | std::ios::binary};

	libv::diff::create_diff(old_, new_, diff, block_size);

	// Close and reopen the file to read
	diff.close();
	diff.open(diff_file_path, std::ios::in | std::ios::binary);

	const auto diff_info = libv::diff::get_diff_info(diff);
	std::cout << "diff_info: " << diff_info.valid() << std::endl;

	if (!diff_info)
		return false;

	std::cout << "diff_info.old_size: " << diff_info.old_size << std::endl;
	std::cout << "diff_info.new_size: " << diff_info.new_size << std::endl;

	const auto check_result = libv::diff::check_diff(old_, new_, diff);

	std::cout << "check_result: " << check_result << std::endl;

	std::ofstream patched{patched_file_path, std::ios::out | std::ios::binary};
	const auto patch_result = libv::diff::apply_patch(old_, diff, patched);

	std::cout << "patch_result: " << patch_result << std::endl;

	if (!patch_result)
		return false;

	return true;
}

// -------------------------------------------------------------------------------------------------

//int main(int, const char**) {
//	const std::string_view old_sv0 = "<libv/utilessed it by yourself or use create_compressed_diff()/patch_decompress() create compressed diffData;\n"
//	                              "//\tif your file size very __ grelkj 999`888 98 large or request faster and less memity/hex_dump.hpp>handle_disconnectb";
//	const std::string_view new_sv0 = "<hdiffpatch/HDiessed it by yourself or use c555()/patch_decompress() create compressed diffData;\n"
//							      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
//                                  "//\tif your file size very __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request faster and less memff/diff.h>handle_disconnecta";
//
//	const std::string_view old_sv1 = "f your file sizef your file size very\0 large or rvery large or r";
//	const std::string_view new_sv1 = "f your file sizef your file size avery\0 large or rvery large or r";
//
//	const std::string_view old_sv2 = "";
//	const std::string_view new_sv2 = "a";
//
//	const std::vector<std::byte> old_bv0 = to_bytes(old_sv0);
//	const std::vector<std::byte> new_bv0 = to_bytes(new_sv0);
//
//	const std::vector<std::byte> old_bv1 = to_bytes(old_sv1);
//	const std::vector<std::byte> new_bv1 = to_bytes(new_sv1);
//
//	const std::vector<std::byte> old_bv2 = to_bytes(old_sv2);
//	const std::vector<std::byte> new_bv2 = to_bytes(new_sv2);
//
//	bool success = true;
//
//	success &= test_memory(old_sv0, new_sv0, 16);
//	success &= test_memory(old_sv1, new_sv1, 16);
//	success &= test_memory(old_sv2, new_sv2, 16);
//
//	success &= test_memory(old_bv0, new_bv0, 16);
//	success &= test_memory(old_bv1, new_bv1, 16);
//	success &= test_memory(old_bv2, new_bv2, 16);
//
//	success &= test_stream(old_sv0, new_sv0, 16);
//	success &= test_stream(old_sv1, new_sv1, 16);
//	success &= test_stream(old_sv2, new_sv2, 16);
//
//	success &= test_file("example_libv_control_hello.exe", "example_libv_control_camera.exe", 16);
//
//	return success ? EXIT_SUCCESS : EXIT_FAILURE;
//}

#include <filesystem>
#include <libv/diff/dir.hpp>
#include <libv/hash/md5.hpp>


using namespace libv::diff;

[[nodiscard]] Manifest create_manifest(const std::string_view root_path) {
	Manifest manifest;

	for(const auto& entry : std::filesystem::recursive_directory_iterator(root_path)) {
		if (not entry.is_regular_file())
			continue;

		const auto& path = entry.path();

		std::ifstream file(path);
		auto md5 = libv::hash::hash_md5(file);

		auto path_str = path.generic_string();

		std::cout << path_str << md5 << std::endl;

		manifest.entries.emplace_back(std::move(path_str), std::move(md5));
	}

	std::cout << manifest.entries.size() << std::endl;

//	libv::sort(manifest.entries, std::less<>{}, &Manifest::Entry::path);
	return manifest;
}

int main() {
	std::filesystem::current_path(std::filesystem::current_path().parent_path());

	create_manifest("src");

	return EXIT_SUCCESS;
}


// -------------------------------------------------------------------------------------------------
