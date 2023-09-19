// Project: libv.re, File: src/libv/re/resource/common_resource_event.hpp

#pragma once

#include <libv/res/resource_path.hpp>

#include <string>
#include <system_error>


namespace libv {
namespace re {

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

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
