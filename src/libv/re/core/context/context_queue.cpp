// Project: libv.re, File: src/libv/re/core/queue_context.cpp

#include <libv/re/core/context/context_queue.hpp>

#include <libv/re/core/scene_internal.hxx>
#include <libv/re/math/bounding_sphere.hpp>


namespace libv::re {

// -------------------------------------------------------------------------------------------------

void ContextQueue::queue(const Light& light) {
	internalScene.queue(light);
}

void ContextQueue::queue(Node* node, const libv::mat4f& matM, Mesh* mesh, Material* material, SubObjectIndex subObjectIndex) {
	internalScene.queue(node, matM, mesh, material, subObjectIndex);
}

std::span<libv::vec2f> ContextQueue::queueDepthQuery(ReadbackQueryIndex& queryIndex, uint32_t count) {
	return internalScene.queueDepthQuery(queryIndex, count);
}

bool ContextQueue::testVisibility(const libv::mat4f& mat, const BoundingSphere& bs) {
	// const libv::frustum_culler_inf& frustum,
	const auto maxScaleFactor = std::sqrt(std::max(
			xyz(mat.row(0)).length_sq(), std::max(
			xyz(mat.row(1)).length_sq(),
			xyz(mat.row(2)).length_sq())));

	const auto positionW = xyz(mat * libv::vec4f(bs.position, 1));
	const auto radiusW = maxScaleFactor * bs.radius;
	const auto visible = frame.viewFrustum.test_sphere(positionW, radiusW);
	return visible;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv::re
