// File:   shader_font.hpp Author: Vader Created on 2019. február 4., 20:32

#pragma once

// libv
#include <libv/glr/uniform.hpp>
// pro
#include <libv/ui/shader/shader.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct ShaderQuad : Shader {
	libv::glr::Uniform_vec4f uniform_color;
	libv::glr::Uniform_mat4f uniform_MVPmat;

	ShaderQuad() {
		vertex(R"(
			#version 330 core

			layout(location = 0) in vec3 vertexPosition;

			uniform mat4 MVPmat;

			void main() {
				gl_Position = MVPmat * vec4(vertexPosition, 1);
			}
		)");
		fragment(R"(
			#version 330 core

			out vec4 output;

			uniform vec4 color;

			void main() {
				output = color;
			}
		)");
		assign(uniform_color, "color", {1, 1, 1, 1});
		assign(uniform_MVPmat, "MVPmat", libv::mat4f::identity());
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
