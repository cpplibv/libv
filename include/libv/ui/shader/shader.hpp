// File:   program_font.hpp Author: Vader Created on 2019. február 4., 20:32

#pragma once

// libv
#include <libv/glr/attribute.hpp>
#include <libv/glr/program.hpp>
#include <libv/glr/uniform_block_binding.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

constexpr auto texture_channel0    = libv::gl::TextureChannel{0};

constexpr auto attribute_position  = libv::glr::Attribute<0, libv::vec3f>{};
constexpr auto attribute_normal    = libv::glr::Attribute<1, libv::vec3f>{};
constexpr auto attribute_color0    = libv::glr::Attribute<2, libv::vec4f>{};
constexpr auto attribute_texture0  = libv::glr::Attribute<8, libv::vec2f>{};

inline const libv::glr::UniformBlockBinding uniform_block_UIInfo{0, "UIInfo"};
//	layout(std140) uniform UIInfo {
//		mat4 matMVP;
//		mat4 matM;
//		vec2 clip_pos;
//		vec2 clip_size;
//		vec2 component_pos;
//		vec2 component_size;
//		vec2 mouse_pos;
//		float time_frame;
//		float time_simulation;
//	};

struct Shader : libv::glr::Program {
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
