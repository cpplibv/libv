// File: model.cpp, Created on 2014. november 28. 17:45, Author: Vader

// hpp
#include <libv/gl/model.hpp>
// ext
#include <GL/glew.h>
#include <boost/asio/buffer.hpp>
#include <boost/filesystem/path.hpp>
// libv
#include <libv/read_file.hpp>
#include <libv/utility.hpp>
#include <libv/vm3/model.hpp>
// std
#include <fstream>
// pro
#include <libv/gl/log.hpp>
#include <libv/gl/model.hpp>
#include <libv/gl/shader.hpp>
#include <libv/gl/uniform.hpp>
#include <libv/gl/gl.hpp>

namespace libv {
namespace gl {

// TODO P2: Model default ctor and load function with the same params as the ctors
// TODO P2: Shader default ctor and load function with the same params as the ctors
// TODO P2: Texture default ctor and load function with the same params as the ctors

// -------------------------------------------------------------------------------------------------

Model::Model(const boost::asio::const_buffer& data, const std::string& name) :
	Model(boost::asio::buffer_cast<const char*>(data), boost::asio::buffer_size(data), name) { }

Model::Model(const boost::filesystem::path& filePath) :
	Model(filePath, filePath.string()) { }

Model::Model(const boost::filesystem::path& filePath, const std::string& name) :
	name(name) {
	const auto data = readFile(filePath, std::ios_base::binary);
	init(data.data(), data.size());
}

Model::Model(const char* data, size_t size, const std::string& name) :
	name(name) {
	init(data, size);
}

void Model::init(const char* data, size_t size) {
	if (!model.load(data, size)) {
		VLOG_ERROR(libv::gl::log(), "Failed to load model: [%s]", name);
		return;
	}
	loadGL();
}

Model::~Model() {
	unloadGL();
}

// -------------------------------------------------------------------------------------------------

void Model::loadGL() {
	VLOG_TRACE(libv::gl::log(), "GL Loading model: [%s]", name);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo_vertex);
	glGenBuffers(1, &vbo_index);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof (vm3::Vertex) * model.vertices.size(), model.vertices.data(), GL_STATIC_DRAW);
	checkGL();

	enableVertexAttribArray(Attribute::position);
	vertexAttribPointer(Attribute::position, 3, VertexAttribType::FLOAT, GL_FALSE, sizeof (vm3::Vertex), MEMBER_OFFSET(vm3::Vertex, position));
	enableVertexAttribArray(Attribute::normal);
	vertexAttribPointer(Attribute::normal, 3, VertexAttribType::FLOAT, GL_FALSE, sizeof (vm3::Vertex), MEMBER_OFFSET(vm3::Vertex, normal));
	enableVertexAttribArray(Attribute::tangent);
	vertexAttribPointer(Attribute::tangent, 3, VertexAttribType::FLOAT, GL_FALSE, sizeof (vm3::Vertex), MEMBER_OFFSET(vm3::Vertex, tangent));
	enableVertexAttribArray(Attribute::bitangent);
	vertexAttribPointer(Attribute::bitangent, 3, VertexAttribType::FLOAT, GL_FALSE, sizeof (vm3::Vertex), MEMBER_OFFSET(vm3::Vertex, bitangent));
	enableVertexAttribArray(Attribute::texcoord0);
	vertexAttribPointer(Attribute::texcoord0, 2, VertexAttribType::FLOAT, GL_FALSE, sizeof (vm3::Vertex), MEMBER_OFFSET(vm3::Vertex, texCoord0));
	enableVertexAttribArray(Attribute::boneindices);
	vertexAttribIPointer(Attribute::boneindices, 4, VertexAttribType::INT, sizeof (vm3::Vertex), MEMBER_OFFSET(vm3::Vertex, boneID));
	enableVertexAttribArray(Attribute::boneweight);
	vertexAttribPointer(Attribute::boneweight, 4, VertexAttribType::FLOAT, GL_FALSE, sizeof (vm3::Vertex), MEMBER_OFFSET(vm3::Vertex, boneWieght));
	checkGL();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (model.indices[0]) * model.indices.size(), model.indices.data(), GL_STATIC_DRAW);
	checkGL();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	checkGL();
}

void Model::unloadGL() {
	VLOG_TRACE(libv::gl::log(), "GL Unloading model: [%s]", name);

	glDeleteBuffers(1, &vbo_index);
	glDeleteBuffers(1, &vbo_vertex);
	glDeleteVertexArrays(1, &vao);
}

void Model::render(libv::gl::GL& gl) {
	glBindVertexArray(vao);
	checkGL();

	renderNode(model.lods[0].rootNodeID, gl);

	glBindVertexArray(0);
	checkGL();
}

void Model::renderNode(uint32_t id, libv::gl::GL& gl) {
	gl.pushMatrixModel();
	gl.matrixModel() *= model.nodes[id].transformation;
//	uniformMmat = gl.matrixModel();
//	uniformMVPmat = gl.matrixMVP(); //<<<Assign uniforms
	//node->material->get<std::string>("diffuseTexture") //<<<Bind Textures here
	//libv::glsl::material = materials[entries[i].MaterialIndex]; //<<<Material here

	if (gl.matrixModel()[0][0] * gl.matrixModel()[1][1] * gl.matrixModel()[2][2] < 0) {
		glFrontFace(GL_CCW);
	} else {
		glFrontFace(GL_CW);
	}

	for (auto meshID : model.nodes[id].meshIDs) {
		glDrawElementsBaseVertex(GL_TRIANGLES,
				model.meshes[meshID].numIndices,
				GL_UNSIGNED_INT,
				(void*) (sizeof (GLuint) * model.meshes[meshID].baseIndex),
				model.meshes[meshID].baseVertex);
		checkGL();
	}

	for (auto childID : model.nodes[id].childrenIDs) {
		renderNode(childID, gl);
	}

	gl.popMatrixModel();
	checkGL();
}

} //namespace gl
} //namespace libv