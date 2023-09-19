// Project: libv.math, File: src/libv/math/aabb.hpp

#pragma once

// pro
#include <libv/math/vec.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

struct AABB {
	libv::vec3f max = {0, 0, 0};
	libv::vec3f min = {0, 0, 0};

public:
	// AABB() = default;

	[[nodiscard]] AABB zero() {
		return AABB{
			.max = libv::vec3f::zero(),
			.min = libv::vec3f::zero()
		};
	}

	[[nodiscard]] AABB empty() {
		return AABB{
			.max = libv::vec3f::neg_inf(),
			.min = libv::vec3f::inf()
		};
	}

public:
	void expand(libv::vec3f vertex) {
		max = libv::vec::max(max, vertex);
		min = libv::vec::min(min, vertex);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
