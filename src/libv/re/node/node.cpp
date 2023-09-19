// Project: libv.re, File: src/libv/re/node/node.cpp

#include <libv/re/node/node.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(Node* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(Node* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

Node::~Node() {
	// For the sake of forward declared types
}

void Node::queue(ContextQueue& ctx) {
	(void) ctx;
}

void Node::queueOverride(ContextQueue& ctx, ContextQueueOverride& ov) {
	(void) ctx;
	(void) ov;
}

void Node::gl_update(ContextUpdate& ctx) {
	(void) ctx;
}

void Node::gl_render(ContextRender& ctx, SubObjectIndex subObjectIndex) {
	(void) ctx;
	(void) subObjectIndex;
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
