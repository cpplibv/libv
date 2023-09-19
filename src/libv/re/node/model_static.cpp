// Project: libv.re, File: src/libv/re/node/model_static.cpp

#include <libv/re/node/model_static.hpp>

#include <libv/re/core/context/context_queue.hpp>
#include <libv/re/core/context/context_queue_override.hpp>
#include <libv/re/resource/model.hpp>


namespace libv::re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(ModelStatic* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(ModelStatic* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

ModelStatic::ModelStatic(const Model_ptr& model) :
	ModelNode(),
	model(model) {
}

ModelStatic::ModelStatic(libv::vec3f position, const Model_ptr& model) :
	ModelNode(position),
	model(model) {
}

ModelStatic::ModelStatic(const Transform& transform, const Model_ptr& model) :
	ModelNode(transform),
	model(model) {
}

void ModelStatic::queue(ContextQueue& ctx) {
	const auto matM = transform.toMatrix();
	if (!ctx.testVisibility(matM, model->boundingSphere()))
		return;

	model->foreachSubObject([&](Mesh* mesh, Material* material, SubObjectIndex subObjectIndex) {
		ctx.queue(this, matM, mesh, material, subObjectIndex);
	});
}

void ModelStatic::queueOverride(ContextQueue& ctx, ContextQueueOverride& ov) {
	const auto matM = transform.toMatrix();
	if (!ctx.testVisibility(matM, model->boundingSphere()))
		return;

	model->foreachSubObject([&](Mesh* mesh, Material* material, SubObjectIndex subObjectIndex) {
		(void) material;
		ctx.queue(this, matM, mesh, ov.model.get(), subObjectIndex);
	});
}

void ModelStatic::gl_update(ContextUpdate& ctx) {
	model->gl_update(ctx);
}

void ModelStatic::gl_render(ContextRender& ctx, SubObjectIndex subObjectIndex) {
	model->gl_render(ctx, subObjectIndex);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv::re
