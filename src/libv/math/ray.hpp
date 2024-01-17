// Project: libv.math, File: src/libv/math/ray.hpp

#pragma once

#include <libv/math/ray_fwd.hpp> // IWYU pragma: export

#include <libv/math/vec.hpp>

#include <cassert>


namespace libv {
// -------------------------------------------------------------------------------------------------

template<std::size_t N, typename T>
struct ray_t {
	libv::vec_t<N, T> position;
	libv::vec_t<N, T> direction;

public:
	ray_t() requires (N == 2) :
		position(0, 0),
		direction(1, 0) {
	}

	ray_t() requires (N == 3) :
		position(0, 0, 0),
		direction(1, 0, 0) {
	}

	ray_t(libv::vec_t<N, T> position, libv::vec_t<N, T> direction) :
		position(position),
		direction(direction) {
		assert(direction.is_normalized());
	}
};

// -------------------------------------------------------------------------------------------------
} // namespace libv
