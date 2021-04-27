// Project: libv, File: app/space/glsl_include_engine.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <functional>
#include <string>
#include <string_view>
// libv
#include <libv/utility/function_ref.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

struct [[nodiscard]] IncludeResult {
	bool success;
	std::string result; /// Error reason or include payload
};

using IncludeLoader = libv::function_ref<IncludeResult(const std::string_view)>;

// -------------------------------------------------------------------------------------------------

[[nodiscard]] std::string load_shader_source(const IncludeLoader& loader, const std::string_view path_main);

// -------------------------------------------------------------------------------------------------

//IncludeResult no_cache_read_file(const std::string_view name) {
//
//}

//struct GLSLIncludeEngine {
//
//};

// -------------------------------------------------------------------------------------------------

//class GLSLIncludeEngine {
//public:
//	using IncludeResult = libv::gl::IncludeResult;
//
//private:
//	IncludeLoader loader;
//
//public:
//	explicit GLSLIncludeEngine(IncludeLoader file_loader);

//	/// Processes the provided source
//	[[nodiscard]] std::string load_source(const std::string_view source, const std::string_view filename = "main");

	/// Uses the file loader to load the given main file and processes it
//	[[nodiscard]] std::string load_main(const std::string_view main_filepath);
//};

// GLSLInspectionEngine
//
//auto ImplGLSLIncludeEngine::extract_inspect(const std::string_view line) const noexcept {
//
// Inspection IDEA GEN 2:
//		Things that inspection should/could store:
//			- Variable name
//			- Variable type
//			- Name - to display on the UI
//			- Description - to display on the UI
//			- Logical Type - to use by value setter in UI
//				- Color
//				- Texture
//				- Range[1234] with min and max value
//				- Float
//				- Vector[1234]
//			- Default value
//
//		#inspect("Foreground color", Color)
//		uniform vec4 color = vec4(0, 0, 0, 0);
//
//		#inspect("name", Range(0, 0.5))
//		uniform vec4 color = vec4(0, 0, 0, 0);
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
//	if (auto m = ctre::match<R"qq(...................)qq">(line)) {
//		m.get<1>().to_view();
//	}
//	} else {
//		return std::nullopt;
//	}
//}


// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
