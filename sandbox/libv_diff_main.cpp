// Project: libv.diff, File: sandbox/libv_diff_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/utility/hex_dump.hpp>
// std
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
// pro
#include <libv/diff/diff.hpp>
#include <libv/diff/manifest.hpp>
#include <libv/diff/manifest_diff.hpp>
#include <libv/hash/md5.hpp>
#include <libv/utility/to_underlying.hpp>


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
	std::memcpy(result.data(), str.data(), str.size());

	return result;
}

// -------------------------------------------------------------------------------------------------

template <typename OT, typename T1, typename T2>
bool test_memory(const T1& old_, const T2& new_, size_t block_size) {
	std::cout << "=================================================================================================" << std::endl;

	const auto diff = libv::diff::create_diff<OT>(old_, new_, block_size);

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

	const auto patch_result = libv::diff::apply_patch<OT>(old_, diff);

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

	// Rewind streams
	old_.clear(std::ios::eofbit); old_.seekg(0);
	new_.clear(std::ios::eofbit); new_.seekg(0);
	diff.clear(std::ios::eofbit); diff.seekg(0);

	const auto check_result = libv::diff::check_diff(old_, new_, diff);

	std::cout << "check_result: " << check_result << std::endl;

	// Rewind streams
	old_.clear(std::ios::eofbit); old_.seekg(0);
	new_.clear(std::ios::eofbit); new_.seekg(0);
	diff.clear(std::ios::eofbit); diff.seekg(0);

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

	// Rewind streams
	old_.clear(std::ios::eofbit); old_.seekg(0);
	new_.clear(std::ios::eofbit); new_.seekg(0);
	diff.clear(std::ios::eofbit); diff.seekg(0);

	const auto check_result = libv::diff::check_diff(old_, new_, diff);

	std::cout << "check_result: " << check_result << std::endl;

	// Rewind streams
	old_.clear(std::ios::eofbit); old_.seekg(0);
	new_.clear(std::ios::eofbit); new_.seekg(0);
	diff.clear(std::ios::eofbit); diff.seekg(0);

	std::ofstream patched{patched_file_path, std::ios::out | std::ios::binary};
	const auto patch_result = libv::diff::apply_patch(old_, diff, patched);

	std::cout << "patch_result: " << patch_result << std::endl;

	if (!patch_result)
		return false;

	return true;
}

// -------------------------------------------------------------------------------------------------

int main(int, const char**) {
	std::filesystem::current_path(std::filesystem::current_path().parent_path());

	const std::string_view old_sv0 = "<libv/utilessed it by yourself or use create_compressed_diff()/patch_decompress() create compressed diffData;\n"
	                              "//\tif your file size very __ grelkj 999`888 98 large or request faster and less memity/hex_dump.hpp>handle_disconnectb";
	const std::string_view new_sv0 = "<hdiffpatch/HDiessed it by yourself or use c555()/patch_decompress() create compressed diffData;\n"
							      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
                                  "//\tif your file size very __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request faster and less memff/diff.h>handle_disconnecta";

	const std::string_view old_sv1 = "f your file sizef your file size very\0 large or rvery large or r";
	const std::string_view new_sv1 = "f your file sizef your file size avery\0 large or rvery large or r";

	const std::string_view old_sv2 = "";
	const std::string_view new_sv2 = "a";

	const std::vector<std::byte> old_bv0 = to_bytes(old_sv0);
	const std::vector<std::byte> new_bv0 = to_bytes(new_sv0);

	const std::vector<std::byte> old_bv1 = to_bytes(old_sv1);
	const std::vector<std::byte> new_bv1 = to_bytes(new_sv1);

	const std::vector<std::byte> old_bv2 = to_bytes(old_sv2);
	const std::vector<std::byte> new_bv2 = to_bytes(new_sv2);

	bool success = true;

	success &= test_memory<std::string>(old_sv0, new_sv0, 16);
	success &= test_memory<std::string>(old_sv1, new_sv1, 16);
	success &= test_memory<std::string>(old_sv2, new_sv2, 16);

	success &= test_memory<std::vector<std::byte>>(old_bv0, new_bv0, 16);
	success &= test_memory<std::vector<std::byte>>(old_bv1, new_bv1, 16);
	success &= test_memory<std::vector<std::byte>>(old_bv2, new_bv2, 16);

	success &= test_memory<std::vector<std::byte>>(old_sv0, new_bv0, 16);
	success &= test_memory<std::string>(old_bv0, new_sv0, 16);

	success &= test_stream(old_sv0, new_sv0, 16);
	success &= test_stream(old_sv1, new_sv1, 16);
	success &= test_stream(old_sv2, new_sv2, 16);

	success &= test_file("bin/example_libv_control_hello.exe", "bin/example_libv_control_camera.exe", 16);

	std::cout << "=================================================================================================" << std::endl;

	const auto manifest = libv::diff::create_manifest("src");

	for (const auto& entry : manifest.directories)
		std::cout << entry.path << std::endl;
	for (const auto& entry : manifest.files)
		std::cout << entry.md5 << " " << entry.path << std::endl;

	std::cout << "=================================================================================================" << std::endl;

	const auto manifest_a = libv::diff::create_manifest("D:/temp/__20201110_143012_CIWoAWXK/HDiffPatch/");
	const auto manifest_b = libv::diff::create_manifest("D:/temp/__20201109_074307_8IqBFnlO/HDiffPatch/");
	const auto manifest_diff = libv::diff::create_manifest_diff(manifest_a, manifest_b);

	for (const auto& entry : manifest_diff.directories)
		std::cout << libv::to_value(entry.change) << " " << entry.filepath << std::endl;
	for (const auto& entry : manifest_diff.files)
		std::cout << libv::to_value(entry.change) << " " << entry.filepath << std::endl;

	std::cout << "=================================================================================================" << std::endl;

	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

// -------------------------------------------------------------------------------------------------
