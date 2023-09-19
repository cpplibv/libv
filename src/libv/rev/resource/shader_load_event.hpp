// Project: libv.rev, File: src/libv/rev/resource/shader_load_event.hpp

#pragma once

// fwd
#include <libv/rev/fwd.hpp>
// libv
#include <optional>
#include <string>
#include <system_error>
#include <vector>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

struct ShaderLoadSuccess {
	ShaderID id = 0;
	const BaseShader& shader;

public:
	// What triggered the (re)load (first reason)
};

struct ShaderLoadFailure {
	struct IncludeFailure {
		// For additional mapping info, use glsl_source_loader's API (ATM it might not exists)

		std::string file_path;
		std::error_code ec;

		std::string include_path;
		/// include_path : line_number
		/// frame at index 0 is the most inner frame
		std::vector<std::pair<std::string, int>> include_stack;
	};

	struct CompileFailure {
		std::string message; /// Failure reason
	};

	struct LinkFailure {
		std::string message; /// Failure reason
	};

public:
	ShaderID id = 0;
	const BaseShader& shader;
	// What triggered the (re)load (first reason)

public:
	std::optional<IncludeFailure> include_failure;
	std::optional<CompileFailure> compile_failure;
	std::optional<LinkFailure> link_failure;
};

struct ShaderUnload {
	ShaderID id = 0;
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
