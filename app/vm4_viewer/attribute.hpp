// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

#pragma once

// libv
#include <libv/glr/attribute.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

constexpr auto attribute_position   = libv::glr::Attribute<0, libv::vec3f>{};
constexpr auto attribute_normal     = libv::glr::Attribute<1, libv::vec3f>{};
constexpr auto attribute_bondID     = libv::glr::Attribute<4, libv::vec4i>{};
constexpr auto attribute_bondWieght = libv::glr::Attribute<5, libv::vec4f>{};
constexpr auto attribute_tangent    = libv::glr::Attribute<6, libv::vec3f>{};
constexpr auto attribute_bitangent  = libv::glr::Attribute<7, libv::vec3f>{};
constexpr auto attribute_texture0   = libv::glr::Attribute<8, libv::vec2f>{};

// -------------------------------------------------------------------------------------------------

} // namespace app
