// File: read_file.hpp

#pragma once

// ext
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
// libv
#include <libv/log/log.hpp>
// std
#include <sstream>

namespace libv {

// -------------------------------------------------------------------------------------------------

inline std::string read_file(const boost::filesystem::path& filePath) {
	std::string result;
	boost::filesystem::ifstream file;
	file.open(filePath.c_str(), std::ios_base::in | std::ios_base::binary);

	if (!file) {
		LIBV_LOG_ERROR("Failed to open file: [%s]", filePath.string());
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
};

inline std::string read_file(const boost::filesystem::path& filePath, ReadFileError& er) {
	std::string result;

	if (filePath.filename().empty()) {
		er = ReadFileError::InvalidPath;
		return result;
	}

	boost::filesystem::ifstream file;
	file.open(filePath.c_str(), std::ios_base::in | std::ios_base::binary);

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

// -------------------------------------------------------------------------------------------------

} // namespace libv
