// Project: libv.rev, File: src/libv/rev/resource/attribute.hpp

#pragma once

// libv
#include <libv/gl/enum.hpp>
#include <libv/glr/attribute.hpp>
#include <libv/math/vec.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

constexpr auto attribute_position   = libv::glr::Attribute<0, libv::vec3f>{};
constexpr auto attribute_normal     = libv::glr::Attribute<1, libv::vec3f>{};
constexpr auto attribute_color0     = libv::glr::Attribute<2, libv::vec4f>{};
constexpr auto attribute_bondID     = libv::glr::Attribute<4, libv::vec4i>{};
constexpr auto attribute_bondWeight = libv::glr::Attribute<5, libv::vec4f>{};
constexpr auto attribute_tangent    = libv::glr::Attribute<6, libv::vec3f>{};
constexpr auto attribute_bitangent  = libv::glr::Attribute<7, libv::vec3f>{};
constexpr auto attribute_texture0   = libv::glr::Attribute<8, libv::vec2f>{};
constexpr auto attribute_custom1    = libv::glr::Attribute<14, libv::vec4f>{};
constexpr auto attribute_custom0    = libv::glr::Attribute<15, libv::vec4f>{};

constexpr auto textureChannel_diffuse = libv::gl::TextureChannel{0};
constexpr auto textureChannel_normal  = libv::gl::TextureChannel{1};
constexpr auto textureChannel_pattern = libv::gl::TextureChannel{7};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
