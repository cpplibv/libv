// Project: libv.ui, File: src/libv/ui/resource/shader_image.hpp

#pragma once

// libv
#include <libv/glr/uniform.hpp>
// pro
#include <libv/ui/resource/shader.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct ShaderImage : Shader {
	libv::glr::Uniform_texture uniform_texture0;

	ShaderImage() {
		vertex(CommonCodeBase + CommonCodeVS + R"(
			layout(location = 0) in vec3 vertexPosition;
			layout(location = 2) in vec4 vertexColor0;
			layout(location = 8) in vec2 vertexTexture0;
			layout(location = 9) in vec4 vertexTexture0Tile;

			out vec4 fragmentColor0;
			out vec2 fragmentTexture0;
			flat out vec4 fragmentTexture0Tile;

			void main() {
				gl_Position = matMVP * vec4(vertexPosition, 1);
				fragmentColor0 = vertexColor0;
				fragmentTexture0 = vertexTexture0;
				fragmentTexture0Tile = vertexTexture0Tile;

				clip_component_vs(matM, vertexPosition, clip_pos, clip_size);
			}
		)");
		fragment(CommonCodeBase + CommonCodeFS + R"(
			in vec4 fragmentColor0;
			in vec2 fragmentTexture0;
			flat in vec4 fragmentTexture0Tile;

			out vec4 result;

			uniform sampler2D samplerTexture0;

			void main() {
				vec4 sample = texture(samplerTexture0, fragmentTexture0Tile.xy + fract(fragmentTexture0) * fragmentTexture0Tile.zw);
				result = sample * fragmentColor0;

				clip_component_fs(result);
			}
		)");

		assign(uniform_texture0, "samplerTexture0", texture_channel0);
		block_binding(uniform_block_UIInfo);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
