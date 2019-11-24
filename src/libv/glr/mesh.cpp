// File:   mesh.cpp Author: Vader Created on 2019. január 25., 6:27

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

	remote = remote_.destroyQueues();
	created = true;
}

void RemoteMesh::update(libv::gl::GL& gl, Remote& remote_) noexcept {
	if (!created)
		create(gl, remote_);

	start = 0;
	count = static_cast<VertexIndex>(indices.data.size());

	gl(vao).bind();

	gl(indices.buffer).bind();
	gl(indices.buffer).data(indices.data, usage);
	gl(vao).bindElements(indices.buffer);

	for (RemoteMeshAttribute& attribute : attributes) {
		gl(attribute.buffer).bind();
		gl(attribute.buffer).data(attribute.data_, usage);
		gl(vao).bindAttribute(attribute.buffer, attribute.channel, attribute.type, attribute.dim, 0, 0);
	}

	dirty = false;
}

void RemoteMesh::render(libv::gl::GL& gl, Remote& remote_) noexcept {
	if (dirty)
		update(gl, remote_);
	else
		gl(vao).bind();

	gl(vao).drawElements(primitive, count, start);
}

void RemoteMesh::render(libv::gl::GL& gl, Remote& remote_, VertexIndex baseVertex, VertexIndex baseIndex, VertexIndex numIndices) noexcept {
	if (dirty)
		update(gl, remote_);
	else
		gl(vao).bind();

//	gl(vao).drawElementsBaseVertex(primitive, baseVertex, baseIndex, numIndices);
	gl(vao).drawElementsBaseVertex(primitive, numIndices, baseIndex, baseVertex);
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
