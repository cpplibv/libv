// Project: libv.re, File: src/libv/re/mesh/mesh_plane.cpp

#include <libv/re/mesh/mesh_plane.hpp>

#include <libv/math/vec.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(MeshPlane* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(MeshPlane* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

MeshPlane::MeshPlane(libv::vec2i divisions, libv::vec2f uvTiles) {
	divisions = libv::vec::max(divisions, 1);
	primitive = libv::gl::Primitive::Triangles;

	using F = float;
	using vec3 = libv::vec3_t<F>;
	using vec2 = libv::vec2_t<F>;
	using index_t = uint32_t;

	const auto numVertices = (divisions.x + 1) * (divisions.y + 1);
	const auto numIndices = divisions.x * divisions.y * 2 * 3;

	struct VertexShadeInfo {
		libv::vec2f uv = libv::uninitialized;
		libv::vec3f normal = libv::uninitialized;
		libv::vec3f tangent = libv::uninitialized;
	};

	auto position = addAttribute(libv::gl::BufferUsage::StaticDraw, attrPosition, numVertices);
	auto vertex = addBuffer<VertexShadeInfo>(libv::gl::BufferUsage::StaticDraw, numVertices);
	addAttribute(vertex, attrUV, &VertexShadeInfo::uv);
	addAttribute(vertex, attrNormal, &VertexShadeInfo::normal);
	addAttribute(vertex, attrTangent, &VertexShadeInfo::tangent);
	auto index = addIndex<index_t>(libv::gl::BufferUsage::StaticDraw, numIndices);

	// Generation:
	const auto divf = divisions.cast<float>();
	const auto invdivf = 1.f / divf;

	index_t vertexIndex = 0;
	for (int32_t y = 0; y < (divisions.y + 1); ++y) {
		const auto py = static_cast<F>(y * 2) * invdivf.y - 1.f;
		const auto v = static_cast<F>(y) * invdivf.y;
		for (int32_t x = 0; x < (divisions.x + 1); ++x) {
			const auto px = static_cast<F>(x * 2) * invdivf.x - 1.f;
			const auto u = static_cast<F>(x) * invdivf.x;

			position[vertexIndex] = vec3(px, py, 0);
			vertex[vertexIndex].uv = vec2(u, v) * uvTiles;
			vertex[vertexIndex].normal = vec3(0, 0, 1);
			vertex[vertexIndex].tangent = vec3(1, 0, 0);
			++vertexIndex;
		}
	}

	index_t indexIndex = 0;
	for (int32_t y = 0; y < divisions.y; ++y) {
		for (int32_t x = 0; x < divisions.x; ++x) {
			const auto i00 = (y + 0) * (divisions.x + 1) + x + 0;
			const auto i10 = (y + 0) * (divisions.x + 1) + x + 1;
			const auto i11 = (y + 1) * (divisions.x + 1) + x + 1;
			const auto i01 = (y + 1) * (divisions.x + 1) + x + 0;

			// TODO P5: Possible to make a Triangle Strip version
			index[indexIndex++] = i00;
			index[indexIndex++] = i10;
			index[indexIndex++] = i11;
			index[indexIndex++] = i11;
			index[indexIndex++] = i01;
			index[indexIndex++] = i00;
		}
	}
}

BoundingSphere MeshPlane::boundingSphere() const {
	return {libv::vec3f{0, 0, 0}, std::sqrt(2.f)};
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
