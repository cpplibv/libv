// File:   read_file.hpp

#pragma once

// ext
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
// vl
#include <vl/log.hpp>

namespace vl {

// -------------------------------------------------------------------------------------------------

inline std::string readFile(const boost::filesystem::path& filePath, const std::ios_base::openmode& mode = std::ios_base::in) {
	std::string result;
	boost::filesystem::ifstream file;
	file.open(filePath.c_str(), std::ios_base::in | mode);

	if (!file) {
		VLOG_ERROR(vl::log(), "Failed to open file: [%s]", filePath.string());
		return result;
	}

	std::ostringstream buffer;
	buffer << file.rdbuf();
	file.close();
	result = buffer.str();

	return result;
}

// -------------------------------------------------------------------------------------------------

} //namespace vl
