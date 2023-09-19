// Project: libv.math, File: src/libv/re/math/transform.hpp

#pragma once

#include <libv/math/vec.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct BoundingSphere {
	libv::vec3f position;
	float radius = 1.f;
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
