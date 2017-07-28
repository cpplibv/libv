// File:   shader_font.hpp Author: Vader Created on 2019. február 4., 20:32

#pragma once

// libv
#include <libv/glr/uniform.hpp>
// pro
#include <libv/ui/shader/shader.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct ShaderImage : Shader {
	libv::glr::Uniform_vec4f uniform_color;
	libv::glr::Uniform_texture uniform_texture;
	libv::glr::Uniform_mat4f uniform_MVPmat;
	constexpr static libv::gl::TextureChannel textureChannel{0};

	ShaderImage() {
		vertex(R"(
			#version 330 core

			layout(location = 0) in vec3 vertexPosition;
			layout(location = 8) in vec2 vertexTexture0;

			out vec2 fragmentTexture0;

			uniform mat4 MVPmat;

			void main() {
				fragmentTexture0 = vertexTexture0;
				gl_Position = MVPmat * vec4(vertexPosition, 1);
			}
		)");
		fragment(R"(
			#version 330 core

			in vec2 fragmentTexture0;

			out vec4 output;

			uniform sampler2D textureSampler;
			uniform vec4 color;

			void main() {
				vec4 sample = texture(textureSampler, fragmentTexture0);
				output = sample * color;
			}
		)");
		assign(uniform_color, "color", {1, 1, 1, 1});
		assign(uniform_MVPmat, "MVPmat", libv::mat4f::identity());
		assign(uniform_texture, "textureSampler", textureChannel);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
