// File: sphere.hpp Author: Vader Created on January 28, 2017, 3:22 PM

#pragma once

// libv
#include <libv/math/constants.hpp>
#include <libv/math/vec.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

template <typename Precision = float, typename Position, typename Normal, typename Texture0, typename Index>
void generatePivot(uint32_t divisions, Position& position, Normal& normal, Texture0& texture0, Index& index) {
	using in = uint32_t;
	using F = Precision;
	using vec2 = libv::vec2_t<Precision>;
	using vec3 = libv::vec3_t<Precision>;
	const auto pi = libv::PI<F>;

	const in ioffset = static_cast<in>(position.size());

	static constexpr vec3 colors[] = { vec3(1, 0, 0), vec3(0, 1, 0), vec3(0, 0, 1) };
	static constexpr vec3 swizzs[] = { vec3(0, 1, 2), vec3(2, 0, 1), vec3(1, 2, 0) };

	for (size_t i = 0; i < 3; ++i) {
		const auto swizle = swizzs[i];
		const auto color = colors[i];

		// Cylinder Base Cap
		for (uint32_t i = 0; i < divisions; ++i) {
			const vec3 p0 = {0, 0, 0};
			const vec3 p1 = {x * F{-0.2}, y * std::sin(i * divisions / pi), y * std::cos(i * divisions / pi)};
			const vec3 p2 = {x * F{-0.2}, y * std::sin((i + 1) * divisions / pi), y * std::cos((i + 1) * divisions / pi)};

			position(p0);
			position(p1[swizle.x], p1[swizle.y], p1[swizle.z]);
			position(p2[swizle.x], p2[swizle.y], p2[swizle.z]);
		}
		// Cylinder
		for (uint32_t i = 0; i < divisions; ++i) {

		}
		// Cone Base Cap
		for (uint32_t i = 0; i < divisions; ++i) {
			const vec3 p0 = {0, 0, 0};
			const vec3 p1 = {x * F{0.8}, y * std::sin(i * divisions / pi), y * std::cos(i * divisions / pi)};
			const vec3 p2 = {x * F{0.8}, y * std::sin((i + 1) * divisions / pi), y * std::cos((i + 1) * divisions / pi)};

			position(p0);
			position(p1[swizle.x], p1[swizle.y], p1[swizle.z]);
			position(p2[swizle.x], p2[swizle.y], p2[swizle.z]);
		}
		// Cone
		for (uint32_t i = 0; i < divisions; ++i) {

		}
	}

//	position(data_position);
//	normal(data_normal);
//	texture0(data_texture0);
//	index(data_index);

	// Adjust indices if geometry was non-empty
	if (ioffset != 0)
		for (auto& i : index.view_last(index_count))
			i += ioffset;
}

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
