// File:   read_file.hpp

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

inline std::string readFile(const boost::filesystem::path& filePath, const std::ios_base::openmode& mode = std::ios_base::in) {
	std::string result;
	boost::filesystem::ifstream file;
	file.open(filePath.c_str(), std::ios_base::in | mode);

	if (!file) {
		LIBV_LIBV_ERROR("Failed to open file: [%s]", filePath.string());
		return result;
	}

	std::ostringstream buffer;
	buffer << file.rdbuf();
	file.close();
	result = buffer.str();

	return result;
}

// -------------------------------------------------------------------------------------------------

} //namespace libv
