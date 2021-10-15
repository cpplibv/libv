// Project: libv.rev, File: src/libv/rev/shader_load_event.hpp

#pragma once

// fwd
#include <libv/rev/fwd.hpp>
// libv
#include <string>
#include <functional>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

struct ShaderLoadSuccess {
	ShaderID id;
	const BaseShader& shader;
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

	ShaderID id;
	const BaseShader& shader;
	// What triggered the (re)load (first reason)

	std::optional<IncludeFailure> include_failure;
	std::optional<CompileFailure> compile_failure;
	std::optional<LinkFailure> link_failure;
};

struct ShaderUnload {
	ShaderID id;
};

using shader_load_success_cb = std::function<void(const ShaderLoadSuccess&)>;
using shader_load_failure_cb = std::function<void(const ShaderLoadFailure&)>;
using shader_unload_cb = std::function<void(const ShaderUnload&)>;

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
