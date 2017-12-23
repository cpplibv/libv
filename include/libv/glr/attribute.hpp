// File: attribute.hpp Author: Vader Created on 2018. december 28., 5:56

#pragma once

// std
#include <cstdint>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

/// constexpr auto attribute_position  = libv::glr::Attribute<0, libv::vec3f>{};
/// constexpr auto attribute_normal    = libv::glr::Attribute<1, libv::vec3f>{};
/// constexpr auto attribute_color0    = libv::glr::Attribute<2, libv::vec4f>{};
/// constexpr auto attribute_tangent   = libv::glr::Attribute<6, libv::vec3f>{};
/// constexpr auto attribute_bitangent = libv::glr::Attribute<7, libv::vec3f>{};
/// constexpr auto attribute_texture0  = libv::glr::Attribute<8, libv::vec2f>{};
///
template <uint32_t Channel, typename T>
struct Attribute {
	using value_type = T;
	static constexpr uint32_t channel = Channel;
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
