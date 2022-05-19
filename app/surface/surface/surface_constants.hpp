// Created by dbobula on 4/19/2022.

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <cmath>


namespace surface {

// -------------------------------------------------------------------------------------------------

constexpr inline auto chunkSideLength = 32.f;
constexpr inline auto chunkSize = libv::vec2f{chunkSideLength, chunkSideLength};

constexpr inline auto chunkRangeScanUpdate = 4;
constexpr inline auto chunkRangeScanInitialization = 5;
constexpr inline auto chunkRangeWindowSize = 6;
constexpr inline auto chunkRangeUnloadRender = chunkRangeWindowSize + 1;

constexpr inline auto worldRangeUnloadRender = chunkRangeUnloadRender * chunkSideLength * std::sqrt(2.f);

static_assert(chunkRangeWindowSize >= chunkRangeScanUpdate);
static_assert(chunkRangeWindowSize >= chunkRangeScanInitialization);
static_assert(chunkRangeWindowSize < chunkRangeUnloadRender);

// -------------------------------------------------------------------------------------------------

} // namespace surface
