// Project: libv.glr, File: src/libv/glr/procedural/sphere.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

// --- Spherified Cube -----------------------------------------------------------------------------

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
			vec3(-1, -1, -1), vec3(2, 0, 0), vec3(0, 2, 0),
			vec3(1, -1, -1), vec3(0, 0, 2), vec3(0, 2, 0),
			vec3(1, -1, 1), vec3(-2, 0, 0), vec3(0, 2, 0),
			vec3(-1, -1, 1), vec3(0, 0, -2), vec3(0, 2, 0),
			vec3(-1, 1, -1), vec3(2, 0, 0), vec3(0, 0, 2),
			vec3(-1, -1, 1), vec3(2, 0, 0), vec3(0, 0, -2)
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

// --- Spherical Geodesic / Icosahedral Grid  ------------------------------------------------------

//void subdivide(const vec3f& v1, const vec3f& v2, const vec3f& v3,
//		std::vector<Vertex>& vertices, const uint32_t depth) {
//	if (depth == 0) {
//		vertices.emplace_back(v1);
//		vertices.emplace_back(v3);
//		vertices.emplace_back(v2);
//		return;
//	}
//	const auto v12 = (v1 + v2).normalize();
//	const auto v23 = (v2 + v3).normalize();
//	const auto v31 = (v3 + v1).normalize();
//	subdivide(v1, v12, v31, vertices, depth - 1);
//	subdivide(v2, v23, v12, vertices, depth - 1);
//	subdivide(v3, v31, v23, vertices, depth - 1);
//	subdivide(v12, v23, v31, vertices, depth - 1);
//}
//
//void sphere_geodesic(uint32_t subdiv) {
//	std::vector<Vertex> vertices;
//	std::vector<uint32_t> indices;
//
//	const float X = 0.525731112119133606f;
//	const float Z = 0.850650808352039932f;
//
//	const vec3f vdata[12] = {
//			{-X, 0.0, Z}, { X, 0.0, Z }, { -X, 0.0, -Z }, { X, 0.0, -Z },
//			{ 0.0, Z, X }, { 0.0, Z, -X }, { 0.0, -Z, X }, { 0.0, -Z, -X },
//			{ Z, X, 0.0 }, { -Z, X, 0.0 }, { Z, -X, 0.0 }, { -Z, -X, 0.0}
//	};
//
//	int tindices[20][3] = {
//			{0, 4, 1}, { 0, 9, 4 }, { 9, 5, 4 }, { 4, 5, 8 }, { 4, 8, 1 },
//			{ 8, 10, 1 }, { 8, 3, 10 }, { 5, 3, 8 }, { 5, 2, 3 }, { 2, 7, 3 },
//			{ 7, 10, 3 }, { 7, 6, 10 }, { 7, 11, 6 }, { 11, 0, 6 }, { 0, 1, 6 },
//			{ 6, 1, 10 }, { 9, 0, 11 }, { 9, 11, 2 }, { 9, 2, 5 }, { 7, 2, 11}
//	};
//
//	for (int i = 0; i < 20; i++)
//		subdivide(vdata[tindices[i][0]], vdata[tindices[i][1]], vdata[tindices[i][2]], vertices, subdiv);
//
//	for (std::size_t i = 0; i < vertices.size(); ++i)
//		indices.push_back(i);
//}

// --- Sphere UV -----------------------------------------------------------------------------------

//void sphere_uv(float radius, uint32_t rings, uint32_t sectors) {
//	std::vector<Vertex> vertices;
//	std::vector<uint32_t> indices;
//
//	const float R = 1.f / (rings - 1);
//	const float S = 1.f / (sectors - 1);
//
//	vertices.reserve(rings * sectors);
//	indices.reserve(rings * sectors * 6);
//
//	for (uint32_t r = 0; r < rings; r++) {
//		for (uint32_t s = 0; s < sectors; s++) {
//			const auto x = std::cos(2 * iris::_PI * s * S) * std::sin(iris::_PI * r * R);
//			const auto y = std::sin(2 * iris::_PI * s * S) * std::sin(iris::_PI * r * R);
//			const auto z = std::sin(-iris::_PI_2 + iris::_PI * r * R);
//
//			vertices.emplace_back(
//					vec3f(x, y, z),
//					vec3(x, y, z),
//					vec2(s * S, r * R)
//					);
//		}
//	}
//
//	for (uint32_t r = 0; r < rings - 1; r++) {
//		for (uint32_t s = 0; s < sectors - 1; s++) {
//			indices.emplace_back(r * sectors + s);
//			indices.emplace_back((r + 1) * sectors + s);
//			indices.emplace_back((r + 1) * sectors + (s + 1));
//			indices.emplace_back((r + 1) * sectors + (s + 1));
//			indices.emplace_back(r * sectors + (s + 1));
//			indices.emplace_back(r * sectors + s);
//		}
//	}
//}

// --- Fibonacci sphere vertices -------------------------------------------------------------------

//std::vector<libv::vec3f> fibonacci_sphere_generator(libv::vec3f a, float radius, int sample_size) {
//	std::vector<libv::vec3f> points;
//	float phi = libv::pi * (3.0f - sqrt(5.0f));  // golden angle in radians
//	for (int i = 0; i < sample_size; ++i) {
//		const float z = 1.0f - (i / (sample_size - 1.0f)) * 2.0f;  // z goes from 1 to -1
//		const float radius0 = sqrt(1 - z * z);  // radius at z
//
//		const float theta = phi * i;  // golden angle increment
//
//		const float x = cos(theta) * radius0;
//		const float y = sin(theta) * radius0;
//		const libv::vec3f point = {x, y, z};
//		points.emplace_back(point * radius + a);
//	}
//	return points;
//}

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
