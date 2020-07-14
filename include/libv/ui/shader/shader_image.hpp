// File:   shader_font.hpp Author: Vader Created on 2019. február 4., 20:32

#pragma once

// libv
#include <libv/glr/uniform.hpp>
//#include <libv/glr/uniform_block_binding.hpp>
// pro
#include <libv/ui/shader/shader.hpp>


// =================================================================================================

//#include <libv/gl/shader_object.hpp>
//#include <libv/gl/program_object.hpp>
//
//#include <libv/gl/program.hpp>
//#include <libv/gl/shader.hpp>
//#include <libv/gl/uniform.hpp>

// =================================================================================================


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct ShaderImage : Shader {
	libv::glr::Uniform_texture uniform_texture0;
//	static constexpr libv::gl::TextureChannel texture_channel0{0};
//	static inline libv::glr::UniformBlockBinding uniform_block_UIInfo{0, "UIInfo"};

	// <<< P8: disable debug view of clipping and use real clipping

	ShaderImage() {
		vertex(R"(
			#version 330 core

			layout(location = 0) in vec3 vertexPosition;
			layout(location = 2) in vec4 vertexColor0;
			layout(location = 8) in vec2 vertexTexture0;

			out vec4 fragmentColor0;
			out vec2 fragmentTexture0;
			out vec4 debug_clip; // <<< P8: better debug control flow

			layout(std140) uniform UIInfo {
				mat4 matMVP;
				mat4 matM;
				vec2 clip_pos;
				vec2 clip_size;
				vec2 component_pos;
				vec2 component_size;
				vec2 mouse_pos;
				float time;
			};

			const bool clip_debug = true; // <<< P8: better debug control flow

			void component_clip(mat4 matM, vec3 vertexPosition, vec2 clip_pos, vec2 clip_size) {
				vec4 position = matM * vec4(vertexPosition, 1);

				debug_clip.x = position.x - clip_pos.x;
				debug_clip.y = position.y - clip_pos.y;
				debug_clip.z = clip_size.x + clip_pos.x - position.x;
				debug_clip.w = clip_size.y + clip_pos.y - position.y;

				// <<< P8: better debug control flow
				if (!clip_debug) {
					gl_ClipDistance[0] = position.x - clip_pos.x;
					gl_ClipDistance[1] = position.y - clip_pos.y;
					gl_ClipDistance[2] = clip_size.x + clip_pos.x - position.x;
					gl_ClipDistance[3] = clip_size.y + clip_pos.y - position.y;
				}
			}

			void main() {
				gl_Position = matMVP * vec4(vertexPosition, 1);
				fragmentColor0 = vertexColor0;
				fragmentTexture0 = vertexTexture0;

				component_clip(matM, vertexPosition, clip_pos, clip_size);
			}
		)");
		fragment(R"(
			#version 330 core

			in vec4 fragmentColor0;
			in vec2 fragmentTexture0;
			in vec4 debug_clip; // <<< P8: better debug control flow

			out vec4 output;

			layout(std140) uniform UIInfo {
				mat4 matMVP;
				mat4 matM;
				vec2 clip_pos;
				vec2 clip_size;
				vec2 component_pos;
				vec2 component_size;
				vec2 mouse_pos;
				float time;
			};

			uniform sampler2D samplerTexture0;

			void main() {
				vec4 sample = texture(samplerTexture0, fragmentTexture0);
				output = sample * fragmentColor0;

				if (debug_clip.x < 0 || debug_clip.y < 0 || debug_clip.z < 0 || debug_clip.w < 0)
					output = sample * vec4(1.5, 0.3, 0.3, 0.75);
			}
		)");

		assign(uniform_texture0, "samplerTexture0", texture_channel0);
		block_binding(uniform_block_UIInfo);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
