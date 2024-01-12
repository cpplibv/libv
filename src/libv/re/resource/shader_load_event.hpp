// Project: libv.re, File: src/libv/re/resource/shader_load_event.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/res/common_resource_event.hpp>

#include <optional>
#include <string>
#include <vector>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct ShaderLoadSuccess {
	ShaderID id = 0;
	Shader_ptr shader;
};

struct ShaderLoadFailure {
	struct IncludeFailure {
		std::string includePath;
		/// includePath : line_number
		/// frame at index 0 is the most outer frame
		std::vector<std::pair<std::string, int>> includeStack;

		std::vector<libv::res::ResourceMappingUnmatched> unmatchedMappings;
		std::vector<libv::res::ResourceMappingError> mappingErrors;
		std::vector<libv::res::LoadFilePhysicalError> physicalErrors;
	};

	struct CompileFailure {
		std::string message; /// Failure reason
	};

	struct LinkFailure {
		std::string message; /// Failure reason
	};

public:
	ShaderID id = 0;
	Shader_ptr shader;

public:
	std::optional<IncludeFailure> includeFailure;
	std::optional<CompileFailure> compileFailure;
	std::optional<LinkFailure> linkFailure;
};

struct ShaderUnload {
	ShaderID id = 0;
	Shader_ptr shader;
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
