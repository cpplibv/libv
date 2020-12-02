// Project: libv.utility, File: src/libv/utility/compare_files.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
//#include <libv/log/log.hpp>
#include <libv/utility/concat.hpp>
// std
#include <filesystem>
#include <fstream>
//#include <sstream>
//#include <string_view>
#include <system_error>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename = void>
[[nodiscard]] bool compare_files_or_throw(const std::filesystem::path& filename1, const std::filesystem::path& filename2) {
	std::ifstream file1(filename1, std::ios::binary | std::ios::in | std::ios::ate); // Open file at the end
	if (!file1)
		throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)), libv::concat("Failed to open file: ", filename1.generic_string()));

	std::ifstream file2(filename2, std::ios::binary | std::ios::in | std::ios::ate); // Open file at the end
	if (!file2)
		throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)), libv::concat("Failed to open file: ", filename2.generic_string()));

	const auto size1 = file1.tellg();
	const auto size2 = file2.tellg();

	if (size1 != size2)
		return false; // Different file size

	file1.seekg(0); // Rewind
	if (file1.fail())
		throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)), libv::concat("Failed to seek file to the beginning: ", filename1.generic_string()));

	file2.seekg(0); // Rewind
	if (file2.fail())
		throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)), libv::concat("Failed to seek file to the beginning: ", filename2.generic_string()));


	static constexpr size_t batch_size = 16 * 1024;
	char buffer1[batch_size];
	char buffer2[batch_size];

//	while(!file1.eof() && !file2.eof()) {
	while(...) {
		file1.read(buffer1, batch_size);
		file2.read(buffer2, batch_size);

		const auto rc1 = file1.gcount();
		const auto rc2 = file2.gcount();

		if (rc1 == rc2) {
//			const auto batch_eq = std::equal(buffer1, buffer1 + rc1, buffer2);
//			std::memcmp(buffer1, buffer1 + rc1, buffer2)
//			...
		} else {
//			(rc1 > rc2 ? file1 : file2).fail()
		}
	}

	// Clear fail bit as we intentionally tried to over-read
//	if (file1.rdstate() == (std::istream::eofbit | std::istream::failbit))
//		file1.clear(std::istream::eofbit);

	return ...;
}

// -------------------------------------------------------------------------------------------------

template <typename = void>
[[nodiscard]] bool compare_files(const std::filesystem::path& filePath, std::error_code& ec) {
//	std::ifstream file(filePath, std::ios::binary | std::ios::in);
//
//	if (!file) {
//		ec.assign(std::make_error_code(static_cast<std::errc>(errno)));
//		return "";
//	}
//
//	std::ostringstream buffer;
//	buffer << file.rdbuf();
//	file.close();
//
//	if (file.fail()) {
//		ec.assign(std::make_error_code(static_cast<std::errc>(errno)));
//		return "";
//	}
//
//	ec.clear();
//	return std::move(buffer).str();

	(void) filePath;
	(void) ec;
	return false;
}

template <typename = void>
[[nodiscard]] auto compare_files_ec(const std::filesystem::path& filePath) {
	struct Result {
		bool equal;
		std::error_code ec;
	};

	Result result;
	result.equal = compare_files(filePath, result.ec);
	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
