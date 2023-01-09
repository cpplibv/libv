// Project: libv.noise, File: src/libv/noise/white.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <cstdint>
#include <vector>


namespace libv {

// -------------------------------------------------------------------------------------------------

//std::vector<double> noise_white_2D_1d(uint64_t seed, uint32_t x, uint32_t y);
std::vector<libv::vec3uc> noise_white_2D_3uc(uint64_t seed, uint32_t x, uint32_t y);

// -------------------------------------------------------------------------------------------------

} // namespace libv
