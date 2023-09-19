// Project: libv.re, File: src/libv/re/mesh/mesh_fullscreen.cpp

#include <libv/re/mesh/mesh_fullscreen.hpp>

#include <libv/math/vec.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(MeshFullscreen* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(MeshFullscreen* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

MeshFullscreen::MeshFullscreen() {
	primitive = libv::gl::Primitive::Triangles;

	using F = float;
	using vec3 = libv::vec3_t<F>;
	using vec2 = libv::vec2_t<F>;
	using index_t = uint32_t;

	const auto numVertices = 3;
	const auto numIndices = 3;

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

	position[0] = vec3(-1, -1, 0);
	position[1] = vec3(3, -1, 0);
	position[2] = vec3(-1, 3, 0);

	vertex[0].uv = vec2(0, 0);
	vertex[1].uv = vec2(2, 0);
	vertex[2].uv = vec2(0, 2);
	vertex[0].normal = vec3(0, 0, 1);
	vertex[1].normal = vec3(0, 0, 1);
	vertex[2].normal = vec3(0, 0, 1);
	vertex[0].tangent = vec3(1, 0, 0);
	vertex[1].tangent = vec3(1, 0, 0);
	vertex[2].tangent = vec3(1, 0, 0);

	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
}

BoundingSphere MeshFullscreen::boundingSphere() const {
	return {libv::vec3f{0, 0, 0}, 1.f};
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
