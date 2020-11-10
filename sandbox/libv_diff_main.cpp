// Project: libv, File: sandbox/libv_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/utility/hex_dump.hpp>
// std
#include <cstring>
#include <iostream>
// pro
#include <libv/diff/diff.hpp>


// -------------------------------------------------------------------------------------------------

std::string_view as_sv(const std::span<const std::byte> s) {
	return std::string_view(reinterpret_cast<const char*>(s.data()), s.size());
}

std::string_view as_sv(const std::string_view s) {
	return s;
}

template <typename T>
bool test(const T& old_, const T& new_, size_t block_size) {

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

// -------------------------------------------------------------------------------------------------

std::vector<std::byte> to_bytes(std::string_view str) {
	std::vector<std::byte> result;

	result.resize(str.size());
	std::memcpy(result.data(), str.data(), result.size());

	return result;
}

int main(int, const char**) {
	const std::string_view old_sv0 = "<libv/utilessed it by yourself or use create_compressed_diff()/patch_decompress() create compressed diffData;\n"
	                              "//\tif your file size very __ grelkj 999`888 98 large or request faster and less memity/hex_dump.hpp>handle_disconnectb";
	const std::string_view new_sv0 = "<hdiffpatch/HDiessed it by yourself or use c555()/patch_decompress() create compressed diffData;\n"
							      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
                                  "//\tif your file size very __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request __ grelkj 999`888 98 large or request faster and less memff/diff.h>handle_disconnecta";

	const std::string_view old_sv1 = "f your file sizef your file size very large or rvery large or r";
	const std::string_view new_sv1 = "f your file sizef your file size avery large or rvery large or r";

	const std::string_view old_sv2 = "";
	const std::string_view new_sv2 = "a";

	const std::vector<std::byte> old_bv0 = to_bytes(old_sv0);
	const std::vector<std::byte> new_bv0 = to_bytes(new_sv0);

	const std::vector<std::byte> old_bv1 = to_bytes(old_sv1);
	const std::vector<std::byte> new_bv1 = to_bytes(new_sv1);

	const std::vector<std::byte> old_bv2 = to_bytes(old_sv2);
	const std::vector<std::byte> new_bv2 = to_bytes(new_sv2);

	bool success = true;

	success &= test(old_sv0, new_sv0, 16);
	success &= test(old_sv1, new_sv1, 16);
	success &= test(old_sv2, new_sv2, 16);

	success &= test(old_bv0, new_bv0, 16);
	success &= test(old_bv1, new_bv1, 16);
	success &= test(old_bv2, new_bv2, 16);

	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

// -------------------------------------------------------------------------------------------------
