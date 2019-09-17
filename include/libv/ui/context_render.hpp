// File: layout_environment.hpp - Created on 2017.10.08. 02:55 - Author: Vader

#pragma once

// libv
#include <libv/gl/enum.hpp>
#include <libv/glr/attribute.hpp>
#include <libv/glr/queue_fwd.hpp>
#include <libv/glr/uniform_block_binding.hpp>
#include <libv/math/vec.hpp>
// pro
#include <libv/ui/chrono.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

constexpr auto attribute_position  = libv::glr::Attribute<0, libv::vec3f>{};
constexpr auto attribute_normal    = libv::glr::Attribute<1, libv::vec3f>{};
constexpr auto attribute_color0    = libv::glr::Attribute<2, libv::vec4f>{};
constexpr auto attribute_tangent   = libv::glr::Attribute<6, libv::vec3f>{};
constexpr auto attribute_bitangent = libv::glr::Attribute<7, libv::vec3f>{};
constexpr auto attribute_texture0  = libv::glr::Attribute<8, libv::vec2f>{};

//const auto uniformBlock_sphere   = libv::glr::UniformBlockBinding{0, "Sphere"};
//const auto uniformBlock_matrices = libv::glr::UniformBlockBinding{1, "Matrices"};
//const auto uniformBlock_material = libv::glr::UniformBlockBinding{2, "Material"};
//const auto uniformBlock_lights   = libv::glr::UniformBlockBinding{3, "Lights"};
//const auto uniformBlock_skeleton = libv::glr::UniformBlockBinding{4, "Skeleton"};

constexpr auto textureChannel_diffuse = libv::gl::TextureChannel{0};
constexpr auto textureChannel_normal  = libv::gl::TextureChannel{1};

struct ContextRender {
	libv::glr::Queue& gl;
	time_point now;
	bool changedLayout = false;   /// Components size or position has changed since last render call
	bool changedPosition = false; /// Components position has changed since last render call
	bool changedSize = false;     /// Components size has changed since last render call
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
