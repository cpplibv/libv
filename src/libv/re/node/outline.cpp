// Project: libv.re, File: src/libv/re/node/outline.cpp

#include <libv/re/node/outline.hpp>

#include <libv/re/core/context/context_queue.hpp>
#include <libv/re/material/internal/material_outline_mask.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(Outline* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(Outline* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

Outline::Outline(Node_ptr node, libv::vec4f color) :
	Proxy(std::move(node)),
	// TODO P4: To not create multiple material of the same type the outline node could accept multiple target nodes
	//			And logically it would make perfect sense
	materials({
		MaterialOutlineMask::create(NodeType::object, color),
		MaterialOutlineMask::create(NodeType::model, color)
	}) {
}

void Outline::queue(ContextQueue& ctx) {
	targetNode->queueOverride(ctx, materials);
}

void Outline::queueOverride(ContextQueue& ctx, ContextQueueOverride& ov) {
	(void) ctx;
	(void) ov;
	// In override mode outline is disabled
}

void Outline::gl_update(ContextUpdate& ctx) {
	assert(false); // Never queued, so gl_update should never be called
	(void) ctx;
}

void Outline::gl_render(ContextRender& ctx, SubObjectIndex subObjectIndex) {
	assert(false); // Never queued, so gl_render should never be called
	(void) subObjectIndex;
	(void) ctx;
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
