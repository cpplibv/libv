// Project: libv.re, File: src/libv/re/core/glsl/glsl_source_code.hpp

#pragma once

#include <libv/container/flat_map.hpp>

#include <map>
#include <string>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct GLSLSourceCode {
	std::string code;
	libv::flat_map<int, std::string> mappingIndexToFilename;

public:
	[[nodiscard]] std::string fixupErrorMessage(std::string_view errorMessage) const;
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
