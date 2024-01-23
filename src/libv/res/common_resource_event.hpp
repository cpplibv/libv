// Project: libv.res, File: src/libv/res/common_resource_event.hpp

#pragma once

#include <libv/res/resource_path.hpp>

#include <string>
#include <system_error>
#include <vector>


namespace libv {
namespace res {

// -------------------------------------------------------------------------------------------------

struct ResourceMappingUnmatched {
	std::string resourcePrefix;
	std::string virtualPrefix;
};

struct ResourceMappingError {
	std::string virtualFilepath;
	libv::res::ResourcePathEC rpec;
};

struct LoadFilePhysicalError {
	std::string physicalFilepath;
	std::error_code ec;
};

struct FileLoadFailure {
	std::vector<ResourceMappingUnmatched> unmatchedMappings;
	std::vector<ResourceMappingError> mappingErrors;
	std::vector<LoadFilePhysicalError> physicalErrors;
};

// -------------------------------------------------------------------------------------------------

} // namespace res
} // namespace libv
