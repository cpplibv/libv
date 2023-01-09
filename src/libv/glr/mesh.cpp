// Project: libv.glr, File: src/libv/glr/mesh.cpp

// hpp
#include <libv/glr/mesh.hpp>
// libv
#include <libv/gl/array_buffer.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/vertex_array.hpp>
// pro
#include <libv/glr/destroy_queue.hpp>
#include <libv/glr/remote.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

void RemoteMesh::create(libv::gl::GL& gl, Remote& remote_) noexcept {
	gl(vao).create();
	gl(indices.buffer).create();
	for (RemoteMeshAttribute& attribute : attributes)
		gl(attribute.buffer).create();

	remote = make_observer_ptr(&remote_.destroyQueues());
	created = true;
}

void RemoteMesh::update(libv::gl::GL& gl, Remote& remote_) noexcept {
	if (!created)
		create(gl, remote_);

	start = 0;
	indexCount = static_cast<VertexIndex>(indices.data.size());

	gl(vao).bind();

	gl(indices.buffer).bind();
	gl(indices.buffer).data(nullptr, indices.data.size() * sizeof(indices.data[0]), usage); // Buffer orphaning
	gl(indices.buffer).subData(indices.data, 0);
	gl(vao).bindElements(indices.buffer);

	for (RemoteMeshAttribute& attribute : attributes) {
		gl(attribute.buffer).bind();
		gl(attribute.buffer).data(nullptr, attribute.data_.size(), usage); // Buffer orphaning
		gl(attribute.buffer).subData(attribute.data_, 0);
		gl(vao).bindAttribute(attribute.buffer, attribute.channel, attribute.type, attribute.dim, 0, 0);
		if (attribute.divisor == 0)
			vertexCount = static_cast<VertexIndex>(attribute.data_.size() / libv::gl::attributeTypeSize(attribute.type) / attribute.dim);
		else
			gl(vao).attributeDivisor(attribute.channel, attribute.divisor);
	}

	dirty = false;
}

void RemoteMesh::render(libv::gl::GL& gl, Remote& remote_) noexcept {
	assert(!attributes.empty() && "Mesh attributes were not initialized");

	if (dirty)
		update(gl, remote_);
	else
		gl(vao).bind();

	gl(vao).drawElements(primitive, start, indexCount);
}

void RemoteMesh::render(libv::gl::GL& gl, Remote& remote_, VertexIndex baseVertex, VertexIndex baseIndex, VertexIndex numIndices) noexcept {
	assert(!attributes.empty() && "Mesh attributes were not initialized");

	if (dirty)
		update(gl, remote_);
	else
		gl(vao).bind();

	gl(vao).drawElementsBaseVertex(primitive, baseVertex, baseIndex, numIndices);
}

void RemoteMesh::renderArraysInstanced(libv::gl::GL& gl, Remote& remote_, int32_t instanceCount) noexcept {
	assert(!attributes.empty() && "Mesh attributes were not initialized");

	if (dirty)
		update(gl, remote_);
	else
		gl(vao).bind();

	gl(vao).drawArraysInstanced(primitive, 0, vertexCount, instanceCount);
}

void RemoteMesh::renderElementsInstanced(libv::gl::GL& gl, Remote& remote_, int32_t instanceCount) noexcept {
	assert(!attributes.empty() && "Mesh attributes were not initialized");

	if (dirty)
		update(gl, remote_);
	else
		gl(vao).bind();

	gl(vao).drawElementsInstanced(primitive, 0, indexCount, instanceCount);
}

RemoteMesh::~RemoteMesh() noexcept {
	if (remote) {
		remote->gc(indices.buffer);
		for (RemoteMeshAttribute& attribute : attributes)
			remote->gc(attribute.buffer);
		remote->gc(vao);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
