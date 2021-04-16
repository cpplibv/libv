// Project: libv.math, File: src/libv/math/noise/blue.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <cstdint>
#include <vector>


namespace libv {

// -------------------------------------------------------------------------------------------------

std::vector<libv::vec3uc> noise_blue_2D_3uc(uint64_t seed, uint32_t X, uint32_t Y);

// -------------------------------------------------------------------------------------------------

} // namespace libv
