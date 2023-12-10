// Project: libv.re, File: src/libv/re/node/object.cpp

#include <libv/re/node/object.hpp>

#include <libv/re/core/context/context_queue.hpp>
#include <libv/re/core/context/context_queue_override.hpp>
#include <libv/re/core/context/context_render.hpp>
#include <libv/re/material/material.hpp>
#include <libv/re/mesh/mesh.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(Object* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(Object* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

Object::~Object() {
	// For the sake of forward declared types
}

void Object::queue(ContextQueue& ctx) {
	const auto matM = transform.toMatrix();
	if (!(material->layers & layerNoFrustumCull).passed() && !ctx.testVisibility(matM, mesh->boundingSphere()))
		return;

	ctx.queue(this, matM, mesh.get(), material.get(), SubObjectIndex{0});
}

void Object::queueOverride(ContextQueue& ctx, ContextQueueOverride& ov) {
	const auto matM = transform.toMatrix();
	if (!(material->layers & layerNoFrustumCull).passed() && !ctx.testVisibility(matM, mesh->boundingSphere()))
		return;

	ctx.queue(this, matM, mesh.get(), ov.object.get(), SubObjectIndex{0});
}

void Object::gl_update(ContextUpdate& ctx) {
	(void) ctx;
}

void Object::gl_render(ContextRender& ctx, SubObjectIndex subObjectIndex) {
	(void) subObjectIndex;
	assert(+subObjectIndex == 0);
	mesh->gl_render(ctx);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
