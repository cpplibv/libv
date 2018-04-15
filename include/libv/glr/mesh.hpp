// File: mesh.hpp - Created on 2017.11.25. 05:06 - Author: Vader

#pragma once

// ext
#include <boost/intrusive_ptr.hpp>
// libv
#include <libv/meta/reflection.hpp>
// std
#include <memory>
#include <vector>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

struct RemoteMesh;

template <typename Vertex>
struct MeshUpdate {
	friend class RemoteMesh;

private:
	using Index = uint32_t;

	libv::gl::Primitive primitive;
	libv::gl::BufferUsage usage;
	std::vector<Vertex> vertices;
	std::vector<Index> indices;

public:
	MeshUpdate(libv::gl::Primitive primitive, libv::gl::BufferUsage usage) :
		primitive(primitive), usage(usage) { }

public:
	template <typename... Args>
	MeshUpdate& vertex(Args&&... args) {
		vertices.emplace_back(Vertex{std::forward<Args>(args)...});
		return *this;
	}

	MeshUpdate& index(Index index_) {
		indices.emplace_back(index_);
		return *this;
	}

	MeshUpdate& index(std::initializer_list<Index> indices_) {
		indices.insert(indices.end(), indices_.begin(), indices_.end());
		return *this;
	}
};

struct RemoteMesh {
	libv::gl::VertexArray vao;
	uint32_t start = 0;
	uint32_t count = 0;
//	uint32_t ref_count = 0;
	libv::gl::Primitive primitive = libv::gl::Primitive::Triangles;
	libv::gl::VertexBuffer vbo_vertex;
	libv::gl::VertexBuffer vbo_index;

	void create() {
		vbo_vertex.create();
		vbo_index.create();
		vao.create();
	}

	template <typename Vertex>
	void update(MeshUpdate<Vertex>&& update_) {
		start = 0;
		count = update_.indices.size();

		auto vao_guard = vao.bind_guard();

		vbo_vertex.data(update_.vertices, update_.usage);
		vbo_index.data(update_.indices, update_.usage);

		Vertex::bindAttributes([this](uint8_t location, auto member_ptr, bool normalized) {
			vao.bindAttribute(vbo_vertex, location, member_ptr, normalized);
		});

		vao.bindElements(vbo_index);
	}

	void render() {
		auto vao_guard = vao.bind_guard();
		vao.drawElements(primitive, count, start);
	}

	void destroy() {
		vao.destroy();
		vbo_vertex.destroy();
		vbo_index.destroy();
	}
};

//inline void intrusive_ptr_add_ref(RemoteMesh* object) {
//	++object->ref_count;
//}
//
//inline void intrusive_ptr_release(RemoteMesh* object) {
//    if(--object->ref_count == 0)
//        delete x;
//}

/// Proxy for a remote mesh
struct Mesh {
private:
	std::shared_ptr<RemoteMesh> rm; // !!! Not here
//	boost::intrusive_ptr<RemoteMesh> object;

public:
	Mesh() {
		rm = std::make_shared<RemoteMesh>();
		rm->create(); // !!! Not here
	}

	~Mesh() {
		if (rm.use_count() == 1)
			rm->destroy(); // !!! Not here
	}

	template <typename Vertex>
	void update(MeshUpdate<Vertex>&& update_) {
		rm->update(std::move(update_));
	}

	void render() { // !!! Not public
		rm->render();
	}
};

struct SubMesh {
	Mesh mesh;
	uint32_t start;
	uint32_t count;
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
