// Project: libv.re, File: src/libv/re/node/light/light.cpp

#include <libv/re/node/light/light.hpp>

#include <libv/re/core/context/context_queue.hpp>
#include <libv/re/math/bounding_sphere.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(Light* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(Light* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

void increase_ref_count(LightDirectional* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(LightDirectional* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

void increase_ref_count(LightPoint* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(LightPoint* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

void increase_ref_count(LightSpot* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(LightSpot* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

void Light::queue(ContextQueue& ctx) {
	const auto effectiveRadius = [&] {
		/// range threshold where the light reaches 0.02 of its original intensity
		const auto threshold = 0.02f;

		switch (attenuationFormula) {
		case LightAttenuationFormula::physical:
			// Physical formula is unbounded, cutoff at threshold
			// Formula: sqr(range) / sqr(dist + range)
			return (1.f / std::sqrt(threshold) - 1.f) * range;
		case LightAttenuationFormula::lisyarus: [[fallthrough]];
		case LightAttenuationFormula::lisyarusSharp:
			return range;
		}
		assert(false && "Invalid LightAttenuationFormula enum value");
		return 0.f;
	}();

	const auto visibleResult = [&] {
		switch (type) {
		case LightType::disable:
			return false;
		case LightType::directional:
			return true;
		case LightType::point: [[fallthrough]];
		case LightType::spot:
			// TODO P2: frustum test with spot lights should use a cone shape test and not a sphere
			return ctx.testVisibility(transform.toMatrix(), BoundingSphere{libv::vec3f{}, effectiveRadius});
		}
		assert(false && "Invalid LightType enum value");
		return false;
	}();

	if (!visibleResult)
		return;

	ctx.queue(*this);
}

void Light::queueOverride(ContextQueue& ctx, ContextQueueOverride& ov) {
	(void) ctx;
	(void) ov;
	// In override mode lights are disabled
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
