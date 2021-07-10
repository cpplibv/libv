// Project: libv.math, File: src/libv/math/ray.hpp, Author: Császár Mátyás [Vader]

#pragma once

// pro
#include <libv/math/vec.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <size_t N, typename T>
struct ray_t {
	libv::vec_t<N, T> position;
	libv::vec_t<N, T> direction;
};

// aliases -----------------------------------------------------------------------------------------

template <typename T> using ray2_t = ray_t<2, T>;
template <typename T> using ray3_t = ray_t<3, T>;

using ray2f = ray2_t<float>;
using ray3f = ray3_t<float>;
using ray2d = ray2_t<double>;
using ray3d = ray3_t<double>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
