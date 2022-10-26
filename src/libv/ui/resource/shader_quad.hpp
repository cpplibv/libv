// Project: libv.ui, File: src/libv/ui/resource/shader_quad.hpp

#pragma once

// libv
#include <libv/glr/uniform.hpp>
// pro
#include <libv/ui/resource/shader.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct ShaderQuad : Shader {
	ShaderQuad() {
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

			out vec4 result;

			void main() {
				result = fragmentColor0;

				clip_component_fs(result);
			}
		)");

		block_binding(uniform_block_UIInfo);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
