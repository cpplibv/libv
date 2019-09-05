// File:   program_font.hpp Author: Vader Created on 2019. február 4., 20:32

#pragma once

// libv
#include <libv/glr/uniform.hpp>
// pro
#include <libv/ui/shader/shader.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct ShaderFont : Shader {
	libv::glr::Uniform_vec4f uniform_color;
	libv::glr::Uniform_mat4f uniform_MVPmat;
	libv::glr::Uniform_texture uniform_texture;
	constexpr static libv::gl::TextureChannel textureChannel{0};

	ShaderFont() {
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

			layout(location = 0, index = 0) out vec4 outputColor;
			layout(location = 0, index = 1) out vec4 outputMask;

			uniform vec4 color;
			uniform sampler2D textureSampler;

			void main() {
				vec3 sample = texture(textureSampler, fragmentTexture0).xyz;
				outputColor = color;
				outputMask = vec4(sample, 1);
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