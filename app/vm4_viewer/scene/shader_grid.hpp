// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

#pragma once

// pro
#include <vm4_viewer/scene/shader.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct UniformsShaderGrid {
	libv::glr::Uniform_vec4f color;
	libv::glr::Uniform_vec3f eye;
	libv::glr::Uniform_mat4f matMVP;

	template <typename Access>
	void update_uniforms(Access& access) {
		access(color, "color");
		access(eye, "eye");
		access(matMVP, "matMVP");
	}
};

using ShaderGrid = app::Shader<UniformsShaderGrid>;

// -------------------------------------------------------------------------------------------------

} // namespace app
