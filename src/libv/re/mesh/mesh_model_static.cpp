// Project: libv.re, File: src/libv/re/mesh/mesh_model_static.cpp

#include <libv/re/mesh/mesh_model_static.hpp>

#include <libv/math/mat.hpp>
#include <libv/vm4/model.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(MeshModelStatic* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(MeshModelStatic* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

MeshModelStatic::MeshModelStatic() {
	primitive = libv::gl::Primitive::Triangles;
}

MeshModelStatic::MeshModelStatic(const libv::vm4::Model& vm4) {
	primitive = libv::gl::Primitive::Triangles;

	using index_t = uint32_t;

	const auto numVertices = vm4.vertices.size();
	const auto numIndices = vm4.indices.size();

	struct VertexShadeInfo {
		libv::vec2f uv = libv::uninitialized;
		libv::vec3f normal = libv::uninitialized;
		libv::vec3f tangent = libv::uninitialized;
		// libv::vec4i boneIndices = libv::uninitialized;
		// libv::vec4f boneWeight = libv::uninitialized;
	};

	auto position = addAttribute(libv::gl::BufferUsage::StaticDraw, attrPosition, numVertices);
	auto vertex = addBuffer<VertexShadeInfo>(libv::gl::BufferUsage::StaticDraw, numVertices);
	addAttribute(vertex, attrUV, &VertexShadeInfo::uv);
	addAttribute(vertex, attrNormal, &VertexShadeInfo::normal);
	addAttribute(vertex, attrTangent, &VertexShadeInfo::tangent);
	auto index = addIndex<index_t>(libv::gl::BufferUsage::StaticDraw, numIndices);

	for (std::size_t i = 0; i < vm4.vertices.size(); ++i) {
		position[i] = vm4.vertices[i].position;
		vertex[i].uv = vm4.vertices[i].texture0;
		vertex[i].normal = vm4.vertices[i].normal;
		vertex[i].tangent = vm4.vertices[i].tangent;
		// vertex[i].boneIndices = vm4.vertices[i].boneID;
		// vertex[i].boneWeight = vm4.vertices[i].boneWeight;
	}
	static_assert(std::is_same_v<index_t, std::remove_cvref_t<decltype(vm4.indices)>::value_type>);
	std::memcpy(index.data(), vm4.indices.data(), vm4.indices.size() * sizeof(index_t));

	boundingSphere_.position = vm4.BS_origin;
	boundingSphere_.radius = vm4.BS_radius;
}

void MeshModelStatic::instanceMatricesRow012(std::span<const libv::mat4x3f> transformsTransposedRow012) {
	struct VertexInstance {
		libv::vec4f instanceMatRow0 = libv::uninitialized;
		libv::vec4f instanceMatRow1 = libv::uninitialized;
		libv::vec4f instanceMatRow2 = libv::uninitialized;
	};

	const auto instanceCount = static_cast<uint32_t>(transformsTransposedRow012.size());
	auto bufferInstance = addBuffer<VertexInstance>(libv::gl::BufferUsage::StaticDraw, instanceCount);
	addInstanceAttribute(bufferInstance, attrInstanceMatRow0, &VertexInstance::instanceMatRow0, 1);
	addInstanceAttribute(bufferInstance, attrInstanceMatRow1, &VertexInstance::instanceMatRow1, 1);
	addInstanceAttribute(bufferInstance, attrInstanceMatRow2, &VertexInstance::instanceMatRow2, 1);

	auto* dst = bufferInstance[0].instanceMatRow0.data();
	const auto* src = transformsTransposedRow012[0].data();
	const auto size = transformsTransposedRow012.size() * sizeof(libv::mat3x4f);
	std::memcpy(dst, src, size);
}

BoundingSphere MeshModelStatic::boundingSphere() const {
	return boundingSphere_;
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
