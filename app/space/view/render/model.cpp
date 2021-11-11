// Project: libv, File: app/space/view/model.cpp

// hpp
#include <space/view/render/model.hpp>
// libv
#include <libv/glr/attribute.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/uniform_buffer.hpp>
#include <libv/vm4/model.hpp>
// pro
//#include <space/renderer.hpp>


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
	auto boneWeight = mesh.attribute(attribute_bondWeight);

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

	boneWeight.reserve(vm4.vertices.size());
	for (const auto& vertex : vm4.vertices)
		boneWeight(vertex.boneWeight);

	index(vm4.indices);
}

void Model::render(libv::glr::Queue& glr, ShaderFleet& shader, libv::glr::UniformBuffer& uniform_stream) {
	render_node(glr, vm4.lods[0].rootNodeID, shader, uniform_stream);
}

void Model::render_node(libv::glr::Queue& glr, uint32_t nodeID, ShaderFleet& shader, libv::glr::UniformBuffer& uniform_stream) {
	const auto guard = glr.model.push_guard();

	glr.model *= vm4.nodes[nodeID].transformation;

	auto uniforms = uniform_stream.block_unique(layout_matrices);
	uniforms[layout_matrices.matMVP] = glr.mvp();
	uniforms[layout_matrices.matM] = glr.model;
	uniforms[layout_matrices.matP] = glr.projection;
	uniforms[layout_matrices.eye] = glr.eye();

	glr.uniform(std::move(uniforms));

	// TODO P2: Bind textures here
	// node->material->get<std::string>("diffuseTexture")

	// TODO P2: Bind material here
	// libv::glsl::material = materials[entries[i].MaterialIndex];

	for (const auto& meshID : vm4.nodes[nodeID].meshIDs)
		glr.render(mesh, vm4.meshes[meshID].baseVertex, vm4.meshes[meshID].baseIndex, vm4.meshes[meshID].numIndices);

	for (auto childID : vm4.nodes[nodeID].childrenIDs)
		render_node(glr, childID, shader, uniform_stream);
}

// -------------------------------------------------------------------------------------------------

} // namespace app
