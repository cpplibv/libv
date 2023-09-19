// Project: libv.re, File: src/libv/re/mesh/mesh_skybox.cpp

#include <libv/re/mesh/mesh_skybox.hpp>

#include <libv/math/vec.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(MeshSkybox* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(MeshSkybox* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

MeshSkybox::MeshSkybox() {
	primitive = libv::gl::Primitive::Triangles;

	using F = float;
	using vec3 = libv::vec3_t<F>;
	using vec2 = libv::vec2_t<F>;
	using index_t = uint32_t;

	const auto numVertices = 6 * 4;
	const auto numIndices = 6 * 3 * 2;

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

	// Inner cube generation:
	struct Face {
		vec3 origin;
		vec3 right;
		vec3 up;
		vec3 normal;
	};
	static const Face faces[6] = {
			//   origin            'U'              'V'                normal
			Face{vec3(+1, +1, +1), vec3(0, 0, -2), vec3(0, -2, 0), vec3(-1, 0, 0)}, // Front
			Face{vec3(+1, +1, +1), vec3(-2, 0, 0), vec3(0, 0, -2), vec3(0, -1, 0)}, // Left
			Face{vec3(+1, +1, +1), vec3(0, -2, 0), vec3(-2, 0, 0), vec3(0, 0, -1)}, // Top

			Face{vec3(-1, -1, -1), vec3(0, +2, 0), vec3(0, 0, +2), vec3(+1, 0, 0)}, // Back
			Face{vec3(-1, -1, -1), vec3(0, 0, +2), vec3(+2, 0, 0), vec3(0, +1, 0)}, // Right
			Face{vec3(-1, -1, -1), vec3(+2, 0, 0), vec3(0, +2, 0), vec3(0, 0, +1)}, // Bottom
	};

	for (int i = 0; i < 6; ++i) {
		const auto& face = faces[i];

		position[i * 4 + 0] = face.origin;
		position[i * 4 + 1] = face.origin + face.right;
		position[i * 4 + 2] = face.origin + face.right + face.up;
		position[i * 4 + 3] = face.origin + face.up;
		vertex[i * 4 + 0].uv = vec2(0, 0);
		vertex[i * 4 + 1].uv = vec2(1, 0);
		vertex[i * 4 + 2].uv = vec2(1, 1);
		vertex[i * 4 + 3].uv = vec2(0, 1);
		vertex[i * 4 + 0].normal = face.normal;
		vertex[i * 4 + 1].normal = face.normal;
		vertex[i * 4 + 2].normal = face.normal;
		vertex[i * 4 + 3].normal = face.normal;
		vertex[i * 4 + 0].tangent = face.right * 0.5f;
		vertex[i * 4 + 1].tangent = face.right * 0.5f;
		vertex[i * 4 + 2].tangent = face.right * 0.5f;
		vertex[i * 4 + 3].tangent = face.right * 0.5f;
	}

	for (int i = 0; i < 6; ++i) {
		index[i * 6 + 0] = i * 4 + 0;
		index[i * 6 + 1] = i * 4 + 1;
		index[i * 6 + 2] = i * 4 + 2;
		index[i * 6 + 3] = i * 4 + 2;
		index[i * 6 + 4] = i * 4 + 3;
		index[i * 6 + 5] = i * 4 + 0;
	}
}

BoundingSphere MeshSkybox::boundingSphere() const {
	return {libv::vec3f{0, 0, 0}, std::sqrt(3.f)};
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
