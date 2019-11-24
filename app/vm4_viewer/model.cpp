// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// hpp
#include <vm4_viewer/model.hpp>
// libv
#include <libv/glr/attribute.hpp>
#include <libv/glr/queue.hpp>
// pro
#include <vm4_viewer/log.hpp>


//#include <libv/utility/generic_path.hpp>
//// std
//#include <filesystem>
//#include <iostream>
//// pro
//#include <vm4_viewer/config.hpp>
//#include <vm4_viewer/frame.hpp>


// -------------------------------------------------------------------------------------------------

namespace app {
namespace {

constexpr auto attribute_position   = libv::glr::Attribute<0, libv::vec3f>{};
constexpr auto attribute_normal     = libv::glr::Attribute<1, libv::vec3f>{};
constexpr auto attribute_bondID     = libv::glr::Attribute<4, libv::vec4i>{};
constexpr auto attribute_bondWieght = libv::glr::Attribute<5, libv::vec4f>{};
constexpr auto attribute_tangent    = libv::glr::Attribute<6, libv::vec3f>{};
constexpr auto attribute_bitangent  = libv::glr::Attribute<7, libv::vec3f>{};
constexpr auto attribute_texture0   = libv::glr::Attribute<8, libv::vec2f>{};

} // namespace

// -------------------------------------------------------------------------------------------------

Model::Model(libv::vm4::Model model) :
	mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw},
	model{std::move(model)} {
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

	position.reserve(model.vertices.size());
	for (const auto& vertex : model.vertices)
		position(vertex.position);

	normal.reserve(model.vertices.size());
	for (const auto& vertex : model.vertices)
		normal(vertex.normal);

	tangent.reserve(model.vertices.size());
	for (const auto& vertex : model.vertices)
		tangent(vertex.tangent);

	bitangent.reserve(model.vertices.size());
	for (const auto& vertex : model.vertices)
		bitangent(vertex.bitangent);

	texture0.reserve(model.vertices.size());
	for (const auto& vertex : model.vertices)
		texture0(vertex.texture0);

	boneID.reserve(model.vertices.size());
	for (const auto& vertex : model.vertices)
		boneID(vertex.boneID);

	boneWieght.reserve(model.vertices.size());
	for (const auto& vertex : model.vertices)
		boneWieght(vertex.boneWieght);

	index(model.indices);
}

void Model::render(libv::glr::Queue& gl) {
	gl.program(shader);

	const auto guard = gl.model.push_guard();

	gl.model.identity();
	node(gl, model.lods[0].rootNodeID);
}

void Model::node(libv::glr::Queue& gl, uint32_t nodeID) {
	const auto guard = gl.model.push_guard();

	gl.model = gl.model * model.nodes[nodeID].transformation;

	gl.uniform(shader.uniform_Mmat, gl.model);
	gl.uniform(shader.uniform_MVPmat, gl.mvp());

	// TODO P2: Bind textures here
	// node->material->get<std::string>("diffuseTexture")

	// TODO P2: Bind material here
	// libv::glsl::material = materials[entries[i].MaterialIndex];

	for (const auto& meshID : model.nodes[nodeID].meshIDs) {
		gl.render(mesh, model.meshes[meshID].baseVertex, model.meshes[meshID].baseIndex, model.meshes[meshID].numIndices);
	}

	for (auto childID : model.nodes[nodeID].childrenIDs) {
		node(gl, childID);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace app
