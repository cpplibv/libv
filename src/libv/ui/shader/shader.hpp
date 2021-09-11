// Project: libv.ui, File: src/libv/ui/shader/shader.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/glr/attribute.hpp>
#include <libv/glr/program.hpp>
#include <libv/glr/uniform_block_binding.hpp>
// std
#include <string>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

constexpr auto texture_channel0        = libv::gl::TextureChannel{0};

constexpr auto attribute_position      = libv::glr::Attribute<0, libv::vec3f>{};
constexpr auto attribute_normal        = libv::glr::Attribute<1, libv::vec3f>{};
constexpr auto attribute_color0        = libv::glr::Attribute<2, libv::vec4f>{};
constexpr auto attribute_texture0      = libv::glr::Attribute<8, libv::vec2f>{};
constexpr auto attribute_texture0_tile = libv::glr::Attribute<9, libv::vec4f>{};

inline const libv::glr::UniformBlockBinding uniform_block_UIInfo{0, "UIInfo"};

struct Shader : libv::glr::Program {
	// TODO P2: Run-time option to enable/disable debug clipping

	static inline const std::string CommonCodeBase = R"(
		#version 330 core

		layout(std140) uniform UIInfo {
			mat4 matMVP;
			mat4 matM;
			vec2 clip_pos;
			vec2 clip_size;
			vec2 component_pos;
			vec2 component_size;
			vec2 mouse_position;
			vec2 ui_size;
			float time;
		};

		const bool debug_clip = true;
	)";

	static inline const std::string CommonCodeVS = R"(
		out vec4 debug_clip_var;

		void clip_component_vs(mat4 matM, vec3 vertexPosition, vec2 clip_pos, vec2 clip_size) {
			vec4 position = vec4(vertexPosition, 1);

			if (debug_clip) {
				debug_clip_var.x = position.x - clip_pos.x;
				debug_clip_var.y = position.y - clip_pos.y;
				debug_clip_var.z = clip_size.x + clip_pos.x - position.x;
				debug_clip_var.w = clip_size.y + clip_pos.y - position.y;

				gl_ClipDistance[0] = 1;
				gl_ClipDistance[1] = 1;
				gl_ClipDistance[2] = 1;
				gl_ClipDistance[3] = 1;

			} else {
				gl_ClipDistance[0] = position.x - clip_pos.x;
				gl_ClipDistance[1] = position.y - clip_pos.y;
				gl_ClipDistance[2] = clip_size.x + clip_pos.x - position.x;
				gl_ClipDistance[3] = clip_size.y + clip_pos.y - position.y;
			}
		}
	)";

	static inline const std::string CommonCodeFS = R"(
		in vec4 debug_clip_var;

		void clip_component_fs(inout vec4 result) {
			if (debug_clip && debug_clip_var.x < 0 || debug_clip_var.y < 0 || debug_clip_var.z < 0 || debug_clip_var.w < 0)
				result = result * vec4(1.5, 0.3, 0.3, 0.6);
		}

		void clip_component_fs(inout vec4 result, inout vec4 mask) {
			if (debug_clip && debug_clip_var.x < 0 || debug_clip_var.y < 0 || debug_clip_var.z < 0 || debug_clip_var.w < 0) {
				result = mix(vec4(0.8, 0.2, 0.2, 0.0), vec4(1.0, 0.5, 0.5, 1.0), result);
				mask = mask * 0.6;
			}
		}
	)";
};


// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
