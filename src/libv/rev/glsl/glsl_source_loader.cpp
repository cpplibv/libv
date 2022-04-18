// Project: libv.rev, File: src/libv/rev/glsl/glsl_source_loader.cpp

// hpp
#include <libv/rev/glsl/glsl_source_loader.hpp>
// libv
#include <libv/algo/slice.hpp>
#include <libv/utility/concat.hpp>
#include <libv/utility/read_file.hpp>
// std
#include <cassert>
#include <string>
#include <vector>
// pro
#include <libv/rev/glsl/glsl_compose.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

struct ImplGLSLSourceLoader {
	using IncludedCB = GLSLSourceLoader::included_cb;
	using GenerateCB = GLSLSourceLoader::generate_cb;

	struct Route {
		std::string include_dir;

		std::string filesystem_dir;
		IncludedCB included_cb;

		GenerateCB generate_cb;

		std::string source_code;

		Route(std::string&& include_dir, std::string&& filesystem_dir, GLSLSourceLoader::included_cb&& included_cb) :
				include_dir(std::move(include_dir)), filesystem_dir(std::move(filesystem_dir)), included_cb(std::move(included_cb)) {}

		Route(std::string&& include_dir, GLSLSourceLoader::generate_cb&& generate_cb) :
				include_dir(std::move(include_dir)), generate_cb(std::move(generate_cb)) {}

		Route(std::string&& include_dir, std::string&& source_code) :
				include_dir(std::move(include_dir)), source_code(std::move(source_code)) {}
	};

public:
	std::vector<Route> mappings;
	std::vector<std::string> global_defines;
	std::vector<std::string> global_includes;

public:
	IncludeResult load(const std::string_view include_path) {
		// TODO P1: security: enforce filesystem scope (requested file must be under the 'root' folder)
		// TODO P5: normalize include_path (to remove things like a/../xyz.glsl -> xyz.glsl)
		IncludeResult result;

		std::vector<const Route*> include_dir_attempts;

		for (const auto& mapping : mappings) {
			if (!include_path.starts_with(mapping.include_dir))
				continue;

			if (!mapping.source_code.empty()) {
				// Mapping is static source code
				result.ec.clear();
				result.result = mapping.source_code;

			} else if (mapping.generate_cb) {
				// Mapping is generate directory
				result.ec.clear();
				result.result = mapping.generate_cb(include_path);

			} else {
				// Mapping is include directory
				const auto file_part = libv::slice_off_view(include_path, mapping.include_dir.size());
				auto file_path = libv::concat(mapping.filesystem_dir, file_part);

				auto read_result = libv::read_file_str_ec(file_path);
				mapping.included_cb(include_path, file_path);

				if (read_result.ec == std::errc::no_such_file_or_directory) {
					// If the file was not found, store lookup attempt and continue with other mappings
					include_dir_attempts.emplace_back(&mapping);
					continue;

				} else if (read_result.ec) {
					result.ec = read_result.ec;
					result.result = std::move(file_path);

				} else {
					result.ec.clear();
					result.result = std::move(read_result.data);
				}
			}

			return result;
		}

		if (!include_dir_attempts.empty()) {
			result.ec = std::make_error_code(std::errc::no_such_file_or_directory);
			result.result = "";

			// TODO P5: Replace result.result abused with a proper error reason report channel (maybe with exceptions from this layer)
			if (include_dir_attempts.size() > 1)
				result.result += "\n";
			for (const auto& attempt : include_dir_attempts) {
				const auto file_part = libv::slice_off_view(include_path, attempt->include_dir.size());
				result.result += libv::concat(attempt->filesystem_dir, file_part, "\n");
			}
			if (include_dir_attempts.size() == 1)
				result.result.pop_back(); // Discard trailing \n
			return result;

		} else {
			result.ec = std::make_error_code(std::errc::argument_out_of_domain);
			result.result = "No include mapping found";
			return result;
		}

		// TODO P5: Detailed report of search paths with include failure information
		//		"Failed to include \"{}\"\n", include_path
		//		"\tMapping \"{}\": {}\n", mapping.include_dir, "Not matches prefix"
		//		"\tMapping \"{}\"->\"{}\": {}\n", mapping.include_dir, mapping.filesystem_dir, "Not matches prefix"
		//		"\tMapping \"{}\"->\"{}\": {}\n", mapping.include_dir, mapping.filesystem_dir, "No such file or directory"
	}
};

// -------------------------------------------------------------------------------------------------

GLSLSourceLoader::GLSLSourceLoader() :
	self(std::make_unique<ImplGLSLSourceLoader>()) {
}

GLSLSourceLoader::~GLSLSourceLoader() {
	// For the sake of forward declared ptr
}

void GLSLSourceLoader::add_global_define(std::string define) {
	self->global_defines.push_back(std::move(define));
}

void GLSLSourceLoader::add_global_include(std::string define) {
	self->global_includes.push_back(std::move(define));
}

void GLSLSourceLoader::add_include_directory(std::string include_dir, std::string filesystem_dir, GLSLSourceLoader::included_cb callback) {
	self->mappings.emplace_back(std::move(include_dir), std::move(filesystem_dir), std::move(callback));
}

void GLSLSourceLoader::add_generate_directory(std::string include_dir, GLSLSourceLoader::generate_cb callback) {
	self->mappings.emplace_back(std::move(include_dir), std::move(callback));
}

void GLSLSourceLoader::add_virtual_source(std::string include_path, std::string source_code) {
	self->mappings.emplace_back(std::move(include_path), std::move(source_code));
}

GLSLSourceCode GLSLSourceLoader::load_source(std::string_view main_path, std::span<const std::string> local_defines, std::span<const std::string> local_includes) {
	std::span<const std::string> defines_list[2]{
			self->global_defines,
			local_defines
	};
	std::span<const std::string> includes_list[2]{
			self->global_includes,
			local_includes
	};

	const auto include_hook = [this](const std::string_view include_path) {
		return self->load(include_path);
	};

	return glsl_compose_from_include(include_hook, main_path, defines_list, includes_list);
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
