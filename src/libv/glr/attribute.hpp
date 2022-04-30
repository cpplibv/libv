// Project: libv.glr, File: src/libv/glr/attribute.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
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

template <uint32_t Channel, typename T>
struct Attribute {
	using value_type = T;
	using underlying_type = T;
	static constexpr uint32_t channel = Channel;
	static constexpr uint32_t dim = 1;
};

template <uint32_t Channel, std::size_t N, typename T>
struct Attribute<Channel, libv::vec_t<N, T>> {
	using value_type = libv::vec_t<N, T>;
	using underlying_type = T;
	static constexpr uint32_t channel = Channel;
	static constexpr uint32_t dim = N;
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
