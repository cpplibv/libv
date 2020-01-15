// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// hpp
#include <vm4_viewer/scene/model.hpp>
// libv
#include <libv/glr/queue.hpp>
// pro
#include <vm4_viewer/attribute.hpp>
#include <vm4_viewer/log.hpp>


// -------------------------------------------------------------------------------------------------

namespace app {

// -------------------------------------------------------------------------------------------------

Model::Model(libv::vm4::Model model) :
	mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw},
	vm4{std::move(model)} {
	load();
}

void Model::load() {
	auto position = mesh.attribute(attribute_position);
	auto normal = mesh.attribute(attribute_normal);
	auto tangent = mesh.attribute(attribute_tangent);
	auto bitangent = mesh.attribute(attribute_bitangent);
	auto texture0 = mesh.attribute(attribute_texture0);
	auto boneID = mesh.attribute(attribute_bondID);
	auto boneWieght = mesh.attribute(attribute_bondWieght);

	auto index = mesh.index();

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

	boneWieght.reserve(vm4.vertices.size());
	for (const auto& vertex : vm4.vertices)
		boneWieght(vertex.boneWieght);

	index(vm4.indices);
}

void Model::render(libv::glr::Queue& gl, ShaderModel& shader) {
	node(gl, vm4.lods[0].rootNodeID, shader);
}

void Model::node(libv::glr::Queue& gl, uint32_t nodeID, ShaderModel& shader) {
	const auto guard = gl.model.push_guard();

	gl.model *= vm4.nodes[nodeID].transformation;

	gl.uniform(shader.uniform.Mmat, gl.model);
	gl.uniform(shader.uniform.MVPmat, gl.mvp());

	// TODO P2: Bind textures here
	// node->material->get<std::string>("diffuseTexture")

	// TODO P2: Bind material here
	// libv::glsl::material = materials[entries[i].MaterialIndex];

	for (const auto& meshID : vm4.nodes[nodeID].meshIDs) {
		gl.render(mesh, vm4.meshes[meshID].baseVertex, vm4.meshes[meshID].baseIndex, vm4.meshes[meshID].numIndices);
	}

	for (auto childID : vm4.nodes[nodeID].childrenIDs) {
		node(gl, childID, shader);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace app
