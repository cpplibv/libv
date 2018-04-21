// File: read_file.hpp

#pragma once

// libv
#include <libv/log/log.hpp>
// std
#include <experimental/filesystem>
namespace std { namespace filesystem = experimental::filesystem; } /*FILESYSTEM_SUPPORT*/
#include <fstream>
#include <sstream>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename = void>
std::string read_file(const std::filesystem::path& filePath) {
	std::string result;
	std::ifstream file(filePath.string() /*FILESYSTEM_SUPPORT*/, std::ios_base::in | std::ios_base::binary);

	if (!file) {
		LIBV_LOG_LIBV_ERROR("Failed to open file: {}", filePath.string());
		return result;
	}

	std::ostringstream buffer;
	buffer << file.rdbuf();
	file.close();
	result = buffer.str();

	return result;
}

// -------------------------------------------------------------------------------------------------

enum class ReadFileError : uint8_t {
	OK = 0,
	FailedOpen,
	//	NotFound,
	InvalidPath,
	//	NoPermission,
	//	AlreadyOpened,
};

template <typename = void>
std::string read_file(const std::filesystem::path& filePath, ReadFileError& er) {
	std::string result;

	if (filePath.filename().empty()) {
		er = ReadFileError::InvalidPath;
		return result;
	}

	std::ifstream file(filePath.string() /*FILESYSTEM_SUPPORT*/, std::ios_base::in | std::ios_base::binary);

	if (!file) {
		er = ReadFileError::FailedOpen;
		return result;
	}

	std::ostringstream buffer;
	buffer << file.rdbuf();
	file.close();
	result = buffer.str();

	er = ReadFileError::OK;
	return result;
}

struct result_read_file {
	std::string content;
	ReadFileError ec;
};

template <typename = void>
result_read_file read_file_ec(const std::filesystem::path& filePath) {
	result_read_file result;
	result.content = read_file(filePath, result.ec);
	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
