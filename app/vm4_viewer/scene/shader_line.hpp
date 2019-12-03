// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

#pragma once

// pro
#include <vm4_viewer/scene/shader.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

// IDEA:
//
// NamedUniform_t + reflection
//
//template <libv::static_string Name, typename Type>
//struct NamedUniform_t : Uniform_t<Type> {
//	static constexpr inline libv::static_string name = Name;
//};
//
//struct UniformsShaderLine {
//	app::Uniform_vec4f<"color"> uniform_color;
//	app::Uniform_mat4f<"matMVP"> uniform_matMVP;
//};

struct UniformsShaderLine {
	libv::glr::Uniform_vec4f uniform_color;
	libv::glr::Uniform_mat4f uniform_matMVP;

	template <typename Access>
	void update_uniforms(Access& access) {
		access(uniform_color, "color");
		access(uniform_matMVP, "matMVP");
	}
};

using ShaderLine = app::Shader<UniformsShaderLine>;

// -------------------------------------------------------------------------------------------------

} // namespace app
