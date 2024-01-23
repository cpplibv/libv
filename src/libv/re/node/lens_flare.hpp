// Project: libv.re, File: src/libv/re/node/lens_flare.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/core/readback_query_index.hpp>
#include <libv/re/node/node.hpp>
#include <libv/re/uniform/uniform_block.hpp>
#include <libv/utility/chrono.hpp>

#include <vector>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class LensFlare : public Node {
public:
	using ptr = LensFlare_ptr;
	using cptr = LensFlare_cptr;

private:
	struct Element {
		MaterialLensFlare_ptr material;
		/// Offset along a line running from the target’s position through the screen center
		/// [0 .. inf]
		///          0 -> Target position
		///       0..1 -> interpolate
		///          1 -> Screen center
		///     1..inf -> extrapolate
		float position = 0.f;
		/// Size in pixels
		float size = 100.f;
		/// Size in pixels when the target is in the corner (farthest it can be)
		///     <0 -> no radial edge scale
		float sizeRadialEdge = -1.f;
		/// [0 .. tau] rotate counter clockwise
		float rotation = 0.f;
		/// 0.0: No center rotation
		/// 1.0: Rotate texture right (X+) toward the center
		float rotateToCenter = 0.f;
		/// [0 .. 1]
		float colorLightAttenuation = 0.f;
		libv::vec4f color = libv::vec4f{1, 1, 1, 1};
	};

	Mesh_ptr mesh;
	std::vector<Element> elements;
	Node_ptr target;

private:
	bool dirty = false;
	bool visibilityUpdated = false;
	float visibilityAnimated = 0; /// [0..1]

	UniformBlock_ptr<UniformBlockLensFlare> materialBlock;

	libv::time_duration_f visibilityFadeTime;
	float visibilityTestTolerance = 0.1f;
	float visibilityDepthDistance = 0;
	libv::vec2f visibilityTestUV; // NOTE: Using a single UV value is a gross over simplification (It should be numSamples * numBuffers UV and using the matching buffer and VP matrix for the result)
	ReadbackQueryIndex visibilityDepthQuery;

public:
	explicit LensFlare(Node_ptr target, libv::time_duration_f visibilityFadeTime = std::chrono::milliseconds(250), float visibilityTestTolerance = 0.1f);

	/// \param texture
	/// \param size
	/// \param position Offset along a line running from the target’s position through the screen center. 0 = Target position, 1 = Screen center.
	/// \param color
	/// \param colorLightAttenuation
	void add(Texture_ptr texture, float position, float size, float sizeRadialEdge = -1.f, libv::vec4f color = {1, 1, 1, 1}, float colorLightAttenuation = 0.f, float rotation = 0.f, bool rotateToCenter = false);

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(Node_ptr target, libv::time_duration_f visibilityFadeTime = std::chrono::milliseconds(250), float visibilityTestTolerance = 0.1f) {
		return ptr::make(std::move(target), visibilityFadeTime, visibilityTestTolerance);
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
