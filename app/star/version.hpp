// Project: libv, File: app/star/version.hpp

#pragma once

// std
#include <string>


namespace star {

// -------------------------------------------------------------------------------------------------

struct BuildInfo {
	std::string version_name;
//	libv::version_number version_number;
//			uint16_t major;
//			uint16_t minor; Non-breaking additions to the API
//			uint16_t patch; Bug fix only no API changes
//	... build_number;
//	... build_hash;
//	... build_date / build_time;
//	... git_commit_number;
//	... git_branch;
//	... variant;
};

extern BuildInfo build;
//extern libv::build_info build;

// -------------------------------------------------------------------------------------------------

} // namespace star
