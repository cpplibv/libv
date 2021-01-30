// Project: libv.math, File: src/libv/math/noise/perlin.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <cstdint>
#include <vector>


namespace libv {

// -------------------------------------------------------------------------------------------------

std::vector<double> noise_perlin_2D(uint64_t seed, uint32_t X, uint32_t Y);

// -------------------------------------------------------------------------------------------------

} // namespace libv
