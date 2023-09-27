// Project: libv.noise, File: src/libv/noise/perlin.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <cstdint>
#include <vector>


namespace libv {

// -------------------------------------------------------------------------------------------------

std::vector<double> noise_texture_perlin2D(uint64_t seed, uint32_t X, uint32_t Y);

std::vector<libv::vec3uc> noise_white_2D_3uc(uint64_t seed, uint32_t x, uint32_t y);

// -------------------------------------------------------------------------------------------------


} // namespace libv
