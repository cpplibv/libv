// Project: libv.math, File: src/libv/math/noise/white.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <cstdint>
#include <vector>


namespace libv {

// -------------------------------------------------------------------------------------------------

//std::vector<double> noise_white_2D_1d(uint64_t seed, uint32_t X, uint32_t Y);
std::vector<libv::vec3uc> noise_white_2D_3uc(uint64_t seed, uint32_t X, uint32_t Y);

// -------------------------------------------------------------------------------------------------

} // namespace libv
