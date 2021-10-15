// Project: libv.glr, File: src/libv/glr/procedural/sphere.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

template <typename Precision = float, typename Position, typename Normal, typename Texture0, typename Index>
void generateSpherifiedCube(uint32_t divisions, Position& position, Normal& normal, Texture0& texture0, Index& index) {
	using F = Precision;
	using vec3 = libv::vec3_t<F>;
	using index_type = typename Index::value_type;

	const auto ioffset = static_cast<index_type>(position.size());
	const auto num_vertex = position.size() + (divisions + 1) * (divisions + 1) * 6;
	const auto num_index = index.size() + divisions * divisions * 2 * 3 * 6;

	position.reserve(num_vertex);
	normal.reserve(num_vertex);
	texture0.reserve(num_vertex);
	index.reserve(num_index);


	divisions = std::max(divisions, 1u);
	const auto step = F{1} / static_cast<F>(divisions);

	static const vec3 faces[6][3] = {
		//     origin             right              up
		vec3(-1, -1, -1), vec3( 2,  0,  0), vec3( 0,  2,  0),
		vec3( 1, -1, -1), vec3( 0,  0,  2), vec3( 0,  2,  0),
		vec3( 1, -1,  1), vec3(-2,  0,  0), vec3( 0,  2,  0),
		vec3(-1, -1,  1), vec3( 0,  0, -2), vec3( 0,  2,  0),
		vec3(-1,  1, -1), vec3( 2,  0,  0), vec3( 0,  0,  2),
		vec3(-1, -1,  1), vec3( 2,  0,  0), vec3( 0,  0, -2)
	};

	const auto k = divisions + 1;
	const auto mid = divisions / 2;
	const auto d = F{1} / static_cast<F>(divisions);

	for (const auto& face : faces) {
		const auto& origin = face[0];
		const auto& right = face[1];
		const auto& up = face[2];

		for (uint32_t j = 0; j <= divisions; j++) {
			const auto jstep = static_cast<F>(j);

			for (uint32_t i = 0; i <= divisions; i++) {
				const auto istep = static_cast<F>(i);

				const vec3 p = origin + step * (istep * right + jstep * up);
				const vec3 p2 = p * p;

				const auto rx = p.x * std::sqrt(F{1} - F{0.5} * (p2.y + p2.z) + p2.y * p2.z / F{3});
				const auto ry = p.y * std::sqrt(F{1} - F{0.5} * (p2.z + p2.x) + p2.z * p2.x / F{3});
				const auto rz = p.z * std::sqrt(F{1} - F{0.5} * (p2.x + p2.y) + p2.x * p2.y / F{3});

				const auto u = istep * d;
				const auto v = jstep * d;

				position(rx, ry, rz);
				normal(rx, ry, rz);
				texture0(u, v);
			}
		}
	}

	const auto calcFaceDir = [&](auto ii, auto jj) {
		if ((divisions % 2 == 1) && (ii == mid || jj == mid))
			return (ii + jj) % 2 == 0;
		else
			return (ii < mid && jj < mid) != (ii > mid - 1 && jj > mid - 1);
	};

	// TODO P5: Possible to make a Triangle Strip version
	for (uint32_t face = 0; face < 6; ++face) {
		for (uint32_t j = 0; j < divisions; ++j) {
			for (uint32_t i = 0; i < divisions; ++i) {
				const auto i00 = (face * k + j) * k + i;
				const auto i10 = (face * k + j + 1) * k + i;
				const auto i11 = (face * k + j + 1) * k + i + 1;
				const auto i01 = (face * k + j) * k + i + 1;

				if (calcFaceDir(i, j))
					index.quad(ioffset + i00, ioffset + i10, ioffset + i11, ioffset + i01);
				else
					index.quad(ioffset + i10, ioffset + i11, ioffset + i01, ioffset + i00);
			}
		}
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
