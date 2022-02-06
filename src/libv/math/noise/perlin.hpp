// Project: libv.math, File: src/libv/math/noise/perlin.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <cstdint>
#include <vector>


namespace libv {

// -------------------------------------------------------------------------------------------------

std::vector<double> noise_texture_perlin2D(uint64_t seed, uint32_t X, uint32_t Y);
float noise_perlin(libv::vec2f coord);
float noise_simplex(libv::vec2f coord);

// -------------------------------------------------------------------------------------------------

} // namespace libv
