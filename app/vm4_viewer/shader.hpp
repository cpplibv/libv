// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

#pragma once

// libv
#include <libv/glr/program.hpp>
#include <libv/glr/uniform.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct ShaderModel : libv::glr::Program {
	libv::glr::Uniform_vec4f uniform_color;
	libv::glr::Uniform_mat4f uniform_Mmat;
	libv::glr::Uniform_mat4f uniform_MVPmat;
	libv::glr::Uniform_texture uniform_texture;
	libv::glr::Uniform_int32 uniform_mode;
	libv::glr::Uniform_float uniform_near;
	libv::glr::Uniform_float uniform_far;

	constexpr static libv::gl::TextureChannel textureChannel{0};

	ShaderModel();
};

// -------------------------------------------------------------------------------------------------

} // namespace app
