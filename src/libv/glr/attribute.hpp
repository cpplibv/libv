// Project: libv.glr, File: src/libv/glr/attribute.hpp

#pragma once

// libv
#include <libv/gl/attribute.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

template <int32_t Channel, typename T>
using Attribute = libv::gl::Attribute<Channel, T>;

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
