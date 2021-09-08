// Project: libv.rev, File: src/libv/rev/glsl_compose.hpp, Author: Császár Mátyás [Vader]

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
	std::string translateErrorMessageFilenames(std::string_view errorMessage) const;
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
