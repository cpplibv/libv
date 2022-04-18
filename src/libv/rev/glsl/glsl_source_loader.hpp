// Project: libv.rev, File: src/libv/rev/glsl/glsl_source_loader.hpp

#pragma once

// std
#include <functional>
#include <memory>
#include <span>
#include <string>
#include <string_view>
// pro
#include <libv/rev/glsl/glsl_source_code.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class GLSLSourceLoader {
public:
	using included_cb = std::function<void(std::string_view include_path, std::string_view file_path)>;
	using generate_cb = std::function<std::string(std::string_view include_path)>;

private:
	std::unique_ptr<class ImplGLSLSourceLoader> self;

public:
	GLSLSourceLoader();
	inline GLSLSourceLoader(GLSLSourceLoader&&) noexcept = default;
	inline GLSLSourceLoader& operator=(GLSLSourceLoader&&) & noexcept = default;
	~GLSLSourceLoader();

public:
	/// Routing is done with prefix matching therefore
	/// include_dir should include the forward slash (/) as last character
	void add_include_directory(std::string include_dir, std::string filesystem_dir, included_cb callback = {});
	/// Routing is done with prefix matching therefore
	/// include_dir should include the forward slash (/) as last character
	void add_generate_directory(std::string include_dir, generate_cb callback);
	/// Routing is done with prefix matching therefore
	/// include_path has to be the full include path
	void add_virtual_source(std::string include_path, std::string source_code);

public:
	void add_global_define(std::string define);
	void add_global_include(std::string include_path);

public:
	/// \throws glsl_failed_include_exception upon failed include
	GLSLSourceCode load_source(std::string_view main_path, std::span<const std::string> defines = {}, std::span<const std::string> includes = {});
};

// -------------------------------------------------------------------------------------------------

// Inspection IDEA GEN 2:
//		Things that shader inspection should/could store:
//			- Define (multi mode shader) inspection
//			- Variable inspection
//		Things that variable inspection should/could store:
//			- Variable name
//			- Variable type
//			- Name - to display on the UI
//			- Description - to display on the UI
//			- Logical Type - to use by value setter in UI
//				- Color
//				- Texture
//				- Range[1234] with min and max value
//				- Float
//				- Vector[234]
//				- Integer
//				? LogRange[1234] with min and max value and a log scale
//				? LogInteger with a log scale
//				? Matrix
//			- Default value
//
//GLSLInspectionEngine
//auto ImplGLSLSourceLoader::extract_inspect(const std::string_view line) const noexcept {
//
//		#inspect("Foreground color", Color)
//		uniform vec4 color = vec4(0, 0, 0, 0);
//
//		#inspect("name", Range(0, 0.5))
//		uniform vec4 color = vec4(0, 0, 0, 0);
//
//		#inspect("name", Vector)
//		uniform vec4 color = vec4(0, 0, 0, 0);
//
//	struct Result {
//		std::string_view name;
//		std::string_view type;
//		std::string_view min;
//		std::string_view max;
//		std::string_view init;
//	};
//
//	if (auto m = ctre::match<R"qq(...)qq">(line)) {
//		m.get<1>().to_view();
//	}
//	} else {
//		return std::nullopt;
//	}
//}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
