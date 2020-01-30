// File: sphere.hpp Author: Vader Created on January 28, 2017, 3:22 PM

#pragma once

// libv
#include <libv/math/constants.hpp>
#include <libv/math/vec.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

struct ProgressSegmentedRingStyle {
	int32_t segment_num = 16; /// >=1
	int32_t resolution = 4; /// >=1

	float up = 0.0f; /// [0..2pi]
//	float segment_fitted = 1.0f; /// [0..1] 0 = segments edges parallel with each other, 1 = segments edges parallel with neighbour edges
//	float segment_curviture = 1.0f; /// [0..1] 0 = outer edges are straight, 1 = outer edges are part of a perfect circle
//	float segment_ratio = 0.8f; /// [0..1) 0 = no gap, 1 = only gap

	float radius_inner = 0.6f; /// [0..inf) 0 = segment starts in the center, 1 = segment start on the outer edge
	float radius_outer = 1.0f; /// [radius_inner..inf] 0 = segment ends in the center

//	bool CW = true;
};

/// \param style
/// \param progress - [0..1]
/// \param position
/// \param normal
/// \param texture0
/// \param index
template <typename Precision = float, typename Position, typename Texture0, typename Index>
void generateProgressSegmentedRing(const ProgressSegmentedRingStyle& style, Position& position, Texture0& texture0, Index& index) {
	using F = Precision;
	using index_type = typename Index::value_type;

	const auto ioffset = static_cast<index_type>(position.size());
	const auto num_vertex = position.size() + style.segment_num * style.resolution * 4;
	const auto num_index = index.size() + style.segment_num * style.resolution * 6;

	position.reserve(num_vertex);
	texture0.reserve(num_vertex);
	index.reserve(num_index);

	const auto two_pi_per_segment = F{2} * libv::PI<F> / static_cast<F>(style.segment_num);

	for (int32_t i = 0; i < style.segment_num; ++i) {
		const auto s0 = two_pi_per_segment * static_cast<F>(i + 0);
		const auto s1 = two_pi_per_segment * static_cast<F>(i + 1);

		for (int32_t j = 0; j < style.resolution; ++j) {
			const auto t0 = (s1 - s0) * static_cast<F>(j + 0) / static_cast<F>(style.resolution) + s0;
			const auto t1 = (s1 - s0) * static_cast<F>(j + 1) / static_cast<F>(style.resolution) + s0;

//			if (style.fill_mode == ProgressSegmentedRingStyle::FillMode::floor)
//			if (F{2} * libv::PI<F> * progress < t1)
//				return;

			const auto sin_t0 = std::sin(style.up + t0);
			const auto sin_t1 = std::sin(style.up + t1);
			const auto cos_t0 = std::cos(style.up + t0);
			const auto cos_t1 = std::cos(style.up + t1);

			position(style.radius_outer * sin_t0, style.radius_outer * cos_t0, 0);
			position(style.radius_inner * sin_t0, style.radius_inner * cos_t0, 0);
			position(style.radius_inner * sin_t1, style.radius_inner * cos_t1, 0);
			position(style.radius_outer * sin_t1, style.radius_outer * cos_t1, 0);

			texture0(t0 / libv::PI<F> / F{2}, F{1});
			texture0(t0 / libv::PI<F> / F{2}, F{0});
			texture0(t1 / libv::PI<F> / F{2}, F{0});
			texture0(t1 / libv::PI<F> / F{2}, F{1});

			index(ioffset + i * style.resolution * 4 + j * 4 + 0);
			index(ioffset + i * style.resolution * 4 + j * 4 + 1);
			index(ioffset + i * style.resolution * 4 + j * 4 + 2);
			index(ioffset + i * style.resolution * 4 + j * 4 + 2);
			index(ioffset + i * style.resolution * 4 + j * 4 + 3);
			index(ioffset + i * style.resolution * 4 + j * 4 + 0);
		}
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
