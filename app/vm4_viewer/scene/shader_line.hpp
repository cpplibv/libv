// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

#pragma once

// pro
#include <vm4_viewer/scene/shader.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct UniformsShaderLine {
	libv::glr::Uniform_mat4f matMVP;
	libv::glr::Uniform_vec4f color;

	template <typename Access>
	void update_uniforms(Access& access) {
		access(color, "color");
		access(matMVP, "matMVP");
	}
};

using ShaderLine = app::Shader<UniformsShaderLine>;

// -------------------------------------------------------------------------------------------------

} // namespace app
