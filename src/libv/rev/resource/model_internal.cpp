// Project: libv.rev, File: src/libv/rev/resource/model_internal.cpp

// hpp
#include <libv/rev/resource/model_internal.hxx>
// libv
#include <libv/glr/queue.hpp>
#include <libv/vm4/model.hpp>
// pro
#include <libv/rev/resource/attribute.hpp>
#include <libv/rev/resource/material_scanner.hpp>
#include <libv/rev/resource/model_loader_internal.hxx>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

void InternalModel::ref_count_one() noexcept {
	if (auto sp = loader_.lock())
		sp->unload(this);
}

// -------------------------------------------------------------------------------------------------

void InternalModel::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
	for (const auto& node : nodes) {
		const auto guard = glr.model.push_guard();
		glr.model *= node.transformation;
		node.material.bind(glr, uniform_stream);
		glr.render(glrMesh, node.baseVertex, node.baseIndex, node.numIndices);
	}
}

// -------------------------------------------------------------------------------------------------

InternalModel::InternalModel(const libv::vm4::Model& model, const Material& mov) :
	glrMesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw} {
	load_mesh(model);
	std::vector<Material> materials(model.materials.size(), mov);
	load_node(model, materials);
}

InternalModel::InternalModel(const libv::vm4::Model& model, const MaterialLoaderFunc& mov) :
	glrMesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw} {

	load_mesh(model);

	std::vector<Material> materials;
	materials.reserve(model.materials.size());
	for (const auto& vm4Material : model.materials)
		materials.emplace_back(mov(vm4Material));

	load_node(model, materials);
}

void InternalModel::load_mesh(const libv::vm4::Model& vm4) {
	glrMesh = libv::glr::Mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

	auto position = glrMesh.attribute(attribute_position);
	auto normal = glrMesh.attribute(attribute_normal);
	auto tangent = glrMesh.attribute(attribute_tangent);
	auto bitangent = glrMesh.attribute(attribute_bitangent);
	auto texture0 = glrMesh.attribute(attribute_texture0);
	auto boneID = glrMesh.attribute(attribute_bondID);
	auto boneWeight = glrMesh.attribute(attribute_bondWeight);

	auto index = glrMesh.index();

//	if({any node}.material->utilizes(model_vertex_attribute::position))
//		create vbo, load data
//	if({any node}.material->utilizes(model_vertex_attribute::normal))
//		create vbo, load data
//	if({any node}.material->utilizes(model_vertex_attribute::tangent))
//		create vbo, load data
//	if({any node}.material->utilizes(model_vertex_attribute::bitangent))
//		create vbo, load data
//	if({any node}.material->utilizes(model_vertex_attribute::texture0))
//		create vbo, load data
//	if({any node}.material->utilizes(model_vertex_attribute::boneID))
//		create vbo, load data
//	if({any node}.material->utilizes(model_vertex_attribute::boneWeight))
//		create vbo, load data

	position.reserve(vm4.vertices.size());
	for (const auto& vertex : vm4.vertices)
		position(vertex.position);

	normal.reserve(vm4.vertices.size());
	for (const auto& vertex : vm4.vertices)
		normal(vertex.normal);

	tangent.reserve(vm4.vertices.size());
	for (const auto& vertex : vm4.vertices)
		tangent(vertex.tangent);

	bitangent.reserve(vm4.vertices.size());
	for (const auto& vertex : vm4.vertices)
		bitangent(vertex.bitangent);

	texture0.reserve(vm4.vertices.size());
	for (const auto& vertex : vm4.vertices)
		texture0(vertex.texture0);

	boneID.reserve(vm4.vertices.size());
	for (const auto& vertex : vm4.vertices)
		boneID(vertex.boneID);

	boneWeight.reserve(vm4.vertices.size());
	for (const auto& vertex : vm4.vertices)
		boneWeight(vertex.boneWeight);

	index(vm4.indices);
}

void InternalModel::load_node(const libv::vm4::Model& vm4, const std::vector<Material>& mats) {
	nodes.clear();
	nodes.reserve(vm4.meshes.size()); // Not precise, meshes could be reused from multiple nodes, but at least this many node will appear
	load_node(vm4, mats, 0, libv::mat4f::identity());
}

void InternalModel::load_node(const libv::vm4::Model& vm4, const std::vector<Material>& mats, int nodeID, const libv::mat4f& parent_transformation) {
	const auto& node = vm4.nodes[nodeID];

	const auto transformation = parent_transformation * node.transformation;

	for (const auto& meshID : node.meshIDs) {
		const auto& mesh = vm4.meshes[meshID];
		nodes.emplace_back(transformation, mats[mesh.materialID], mesh.baseVertex, mesh.baseIndex, mesh.numIndices);
	}

	for (auto childID : node.childrenIDs)
		load_node(vm4, mats, childID, transformation);
}


// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
