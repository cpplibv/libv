// File: write_file.hpp, Created on 2018.07.14. 02:23, Author: Vader

#pragma once

// std
#include <experimental/filesystem>
namespace std { namespace filesystem = experimental::filesystem; } /*FILESYSTEM_SUPPORT*/
#include <fstream>
#include <string_view>
#include <system_error>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename = void>
void write_file_or_throw(const std::filesystem::path& filePath, std::string_view data) {
	std::ofstream file(filePath.string() /*FILESYSTEM_SUPPORT*/, std::ios_base::out | std::ios_base::binary);

	if (!file)
		throw std::system_error(errno, std::system_category(), fmt::format("Failed to open file: {}", filePath.string()));

	file.write(data.data(), data.size());

	if (file.bad())
		throw std::system_error(errno, std::system_category(), fmt::format("Failed to write file: {}", filePath.string()));
}

// -------------------------------------------------------------------------------------------------

template <typename = void>
void write_file(const std::filesystem::path& filePath, std::string_view data, std::error_code& ec) {
	std::ofstream file(filePath.string() /*FILESYSTEM_SUPPORT*/, std::ios_base::out | std::ios_base::binary);

	if (!file) {
		ec.assign(errno, std::system_category());
		return;
	}

	file.write(data.data(), data.size());

	if (file.bad()) {
		ec.assign(errno, std::system_category());
		return;
	}

	ec.clear();
}

template <typename = void>
[[nodiscard]] std::error_code write_file_ec(const std::filesystem::path& filePath, std::string_view data) {
	std::error_code ec;
	write_file(filePath, data, ec);
	return ec;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
