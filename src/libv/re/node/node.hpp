// Project: libv.re, File: src/libv/re/node/node.hpp

#pragma once

#include <libv/math/quat.hpp>
#include <libv/math/vec.hpp>
#include <libv/re/fwd.hpp>
#include <libv/re/common/node_type.hpp>
#include <libv/re/common/sub_object_index.hpp>
#include <libv/re/math/transform.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class Node : public libv::ref_count_base {
public:
	friend libv::ref_count_access;
	using ptr = Node_ptr;
	using cptr = Node_cptr;

public:
	NodeType nodeType;
private:
	friend SceneInternal;
	static_assert(sizeof(NodeType) == 1);
	bool updatePending = false;
	bool visible_ = true;
	//< 1 byte padding
public:
	Transform transform;

public:
	explicit Node(NodeType nodeType) :
		nodeType(nodeType) { }
	Node(NodeType nodeType, const Transform& transform) :
		nodeType(nodeType),
		transform(transform) { }
	Node(NodeType nodeType, libv::vec3f position, libv::quatf orientation, libv::vec3f scale = {1, 1, 1}) :
		nodeType(nodeType),
		transform(position, orientation, scale) { }
	Node(NodeType nodeType, libv::vec3f position, libv::vec3f direction = {1, 0, 0}, libv::vec3f up = {0, 0, 1}, libv::vec3f scale = {1, 1, 1}) :
		nodeType(nodeType),
		transform(position, libv::quatf::look_at(direction, up), scale) { }
	virtual ~Node();

public:
	constexpr inline void visible(bool value) noexcept {
		visible_ = value;
	}
	[[nodiscard]] constexpr inline bool visible() const noexcept {
		return visible_;
	}

public:
	// hierarchical:
	// 	queue
	// 	queueOverride
	// non-hierarchical:
	// 	gl_update
	// 	gl_render

	virtual void queue(ContextQueue& ctx);
	virtual void queueOverride(ContextQueue& ctx, ContextQueueOverride& ov);
	virtual void gl_update(ContextUpdate& ctx);
	virtual void gl_render(ContextRender& ctx, SubObjectIndex subObjectIndex);
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
