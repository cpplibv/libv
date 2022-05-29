// Project: libv.rev, File: src/libv/rev/glsl/glsl_source_code.hpp

#pragma once

// std
#include <map>
#include <string>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

struct GLSLSourceCode {
	std::string code;
	std::map<int, std::string> mappingIndexToFilename;

public:
	[[nodiscard]] std::string translateErrorMessageFilenames(std::string_view errorMessage) const;
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
