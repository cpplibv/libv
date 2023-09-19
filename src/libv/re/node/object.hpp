// Project: libv.re, File: src/libv/re/node/object.hpp

#pragma once

#include <libv/math/vec.hpp>
#include <libv/re/fwd.hpp>
#include <libv/re/math/transform.hpp>
#include <libv/re/node/node.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class Object : public Node {
public:
	friend libv::ref_count_access;
	using ptr = Object_ptr;
	using cptr = Object_cptr;

public:
	Material_ptr material;
	Mesh_ptr mesh;

protected:
	Object() :
		Node(NodeType::object) { }
public:
	Object(libv::vec3f position, Material_ptr material, Mesh_ptr mesh) :
		Node(NodeType::object, position),
		material(std::move(material)), mesh(std::move(mesh)) { }
	Object(const Transform& transform, Material_ptr material, Mesh_ptr mesh) :
		Node(NodeType::object, transform),
		material(std::move(material)), mesh(std::move(mesh)) { }
	virtual ~Object();

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(const Transform& transform, Material_ptr material, Mesh_ptr mesh) {
		return ptr::make(transform, std::move(material), std::move(mesh));
	}
	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(libv::vec3f position, Material_ptr material, Mesh_ptr mesh) {
		return ptr::make(position, std::move(material), std::move(mesh));
	}

public:
	virtual void queue(ContextQueue& ctx) override;
	virtual void queueOverride(ContextQueue& ctx, ContextQueueOverride& ov) override;
	virtual void gl_update(ContextUpdate& ctx) override;
	virtual void gl_render(ContextRender& ctx, SubObjectIndex subObjectIndex) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
