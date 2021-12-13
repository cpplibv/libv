// Project: libv.ui, File: src/libv/ui/shader/shader_font.hpp

#pragma once

// libv
#include <libv/glr/uniform.hpp>
// pro
#include <libv/ui/shader/shader.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct ShaderFont : Shader {
	libv::glr::Uniform_texture uniform_texture0;

	ShaderFont() {
		vertex(CommonCodeBase + CommonCodeVS + R"(
			layout(location = 0) in vec3 vertexPosition;
			layout(location = 2) in vec4 vertexColor0;
			layout(location = 8) in vec2 vertexTexture0;

			out vec4 fragmentColor0;
			out vec2 fragmentTexture0;

			void main() {
				gl_Position = matMVP * vec4(vertexPosition, 1);
				fragmentColor0 = vertexColor0;
				fragmentTexture0 = vertexTexture0;

				clip_component_vs(matM, vertexPosition, clip_pos, clip_size);
			}
		)");
		fragment(CommonCodeBase + CommonCodeFS + R"(
			in vec4 fragmentColor0;
			in vec2 fragmentTexture0;

			layout(location = 0, index = 0) out vec4 outputColor;
			layout(location = 0, index = 1) out vec4 outputMask;

			uniform sampler2D samplerTexture0;

			void main() {
				ivec2 size = textureSize(samplerTexture0, 0);
				vec3 sample = texture(samplerTexture0, fragmentTexture0 / size).xyz;
				outputColor = fragmentColor0;
				outputMask = vec4(sample, 1) * fragmentColor0.a;

				clip_component_fs(outputColor, outputMask);
			}
		)");

		assign(uniform_texture0, "samplerTexture0", texture_channel0);
		block_binding(uniform_block_UIInfo);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
