// File: read_file.hpp, Created on 2017.04.14. 02:10, Author: Vader

#pragma once

// libv
#include <libv/log/log.hpp>
// std
#include <experimental/filesystem>
namespace std { namespace filesystem = experimental::filesystem; } /*FILESYSTEM_SUPPORT*/
#include <fstream>
#include <sstream>
#include <string_view>
#include <system_error>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename = void>
[[nodiscard]] std::string read_file_or_empty(const std::filesystem::path& filePath) {
	std::string result;
	std::ifstream file(filePath.string() /*FILESYSTEM_SUPPORT*/, std::ios_base::in | std::ios_base::binary);

	if (!file) {
		log.error("Failed to open file: {}", filePath.string());
		return result;
	}

	std::ostringstream buffer;
	buffer << file.rdbuf();
	file.close();
	result = buffer.str();

	return result;
}

// -------------------------------------------------------------------------------------------------

template <typename = void>
[[nodiscard]] std::string read_file_or_throw(const std::filesystem::path& filePath) {
	std::string result;
	std::ifstream file(filePath.string() /*FILESYSTEM_SUPPORT*/, std::ios_base::in | std::ios_base::binary);

	if (!file)
		throw std::system_error(errno, std::system_category(), fmt::format("Failed to open file: {}", filePath.string()));

	std::ostringstream buffer;
	buffer << file.rdbuf();
	file.close();
	result = buffer.str();

	if (file.bad())
		throw std::system_error(errno, std::system_category(), fmt::format("Failed to read file: {}", filePath.string()));

	return result;
}

// -------------------------------------------------------------------------------------------------

template <typename = void>
[[nodiscard]] std::string read_file(const std::filesystem::path& filePath, std::error_code& ec) {
	std::string result;
	std::ifstream file(filePath.string() /*FILESYSTEM_SUPPORT*/, std::ios_base::in | std::ios_base::binary);

	if (!file) {
		ec.assign(errno, std::system_category());
		return result;
	}

	std::ostringstream buffer;
	buffer << file.rdbuf();
	file.close();
	result = buffer.str();

	if (file.bad()) {
		ec.assign(errno, std::system_category());
		return result;
	}

	ec.clear();
	return result;
}

struct result_read_file {
	std::string data;
	std::error_code ec;
};

template <typename = void>
[[nodiscard]] result_read_file read_file_ec(const std::filesystem::path& filePath) {
	result_read_file result;
	result.data = read_file(filePath, result.ec);
	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
