// Project: libv.re, File: src/libv/re/node/lens_Flare.cpp

#include <libv/re/node/lens_flare.hpp>

#include <libv/math/mix.hpp>
#include <libv/re/core/context/context_queue.hpp>
#include <libv/re/core/context/context_render.hpp>
#include <libv/re/material/material_lens_flare.hpp>
#include <libv/re/mesh/mesh.hpp>
#include <libv/re/node/light/light.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(LensFlare* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(LensFlare* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

LensFlare::LensFlare(Node_ptr target, libv::time_duration_f visibilityFadeTime, float visibilityTestTolerance) :
	Node(NodeType::object),
	target(std::move(target)),
	materialBlock(UniformBlock_ptr<UniformBlockLensFlare>::create()),
	visibilityFadeTime(visibilityFadeTime),
	visibilityTestTolerance(visibilityTestTolerance) {
	assert(this->target);
	mesh = Mesh::create();
}

// -------------------------------------------------------------------------------------------------

void LensFlare::add(Texture_ptr texture, float position, float size, float sizeRadialEdge, libv::vec4f color, float colorLightAttenuation, float rotation, bool rotateToCenter) {
	dirty = true;
	MaterialLensFlare_ptr material = nullptr;
	for (const auto& element : elements)
		if (element.material->texture.texture == texture)
			material = element.material;
	if (!material)
		material = MaterialLensFlare::create(std::move(texture), materialBlock);

	elements.emplace_back(
			std::move(material),
			position,
			size,
			sizeRadialEdge,
			rotation,
			rotateToCenter ? 1.f : 0.f,
			colorLightAttenuation,
			color);
}

// -------------------------------------------------------------------------------------------------

void LensFlare::queue(ContextQueue& ctx) {
	const auto matM = transform.toMatrix();
	const auto isLight = target->nodeType == NodeType::light;
	const auto isDirectionalLight = isLight && static_cast<const Light&>(*target).type == LightType::directional;

	const auto targetPositionW = isDirectionalLight ?
			libv::vec4f(-target->transform.rotation.forward(), 0) :
			libv::vec4f(target->transform.position, 1);
			// TODO P1: target->getWorldPosition(); for hierarchical position
			//		\ (ctx.matWorld * matM).column(3).xyz is incorrect, as matWorld is not the target's matWorld

	const auto [targetPositionUV, ahead] = ctx.frame.screenPicker.to_pixel_uv_ahead(targetPositionW);
	const auto pixelSizeUV = ctx.frame.screenPicker.pixel_uv_size();
	const auto onScreen = ahead && libv::vec::within(targetPositionUV, {0, 0}, {1, 1});

	visibilityDepthDistance = isDirectionalLight ?
			std::numeric_limits<float>::infinity() :
			(xyz(targetPositionW) - ctx.frame.eyeW).length();
	visibilityTestUV = targetPositionUV;

	if (!ahead)
		visibilityAnimated = 0; // Instant fade out if target gets behind the camera, so flares don't flip around

	if (onScreen) {
		auto uvs = ctx.queueDepthQuery(visibilityDepthQuery, 13);
		// Sample pattern:
		//            *           ^
		//        *       *       |
		//          *   *         |
		//      *     *     *     | 7 px
		//          *   *         |
		//        *       *       |
		//            *           v
		uvs[0] = targetPositionUV;
		uvs[1] = targetPositionUV + libv::vec2f(+3, +0) * pixelSizeUV;
		uvs[2] = targetPositionUV + libv::vec2f(+2, +2) * pixelSizeUV;
		uvs[3] = targetPositionUV + libv::vec2f(+0, +3) * pixelSizeUV;
		uvs[4] = targetPositionUV + libv::vec2f(-2, +2) * pixelSizeUV;
		uvs[5] = targetPositionUV + libv::vec2f(-3, +0) * pixelSizeUV;
		uvs[6] = targetPositionUV + libv::vec2f(-2, -2) * pixelSizeUV;
		uvs[7] = targetPositionUV + libv::vec2f(+0, -3) * pixelSizeUV;
		uvs[8] = targetPositionUV + libv::vec2f(+2, -2) * pixelSizeUV;
		uvs[9] = targetPositionUV + libv::vec2f(-1, -1) * pixelSizeUV;
		uvs[10] = targetPositionUV + libv::vec2f(+1, -1) * pixelSizeUV;
		uvs[11] = targetPositionUV + libv::vec2f(+1, +1) * pixelSizeUV;
		uvs[12] = targetPositionUV + libv::vec2f(-1, +1) * pixelSizeUV;
	}

	if (!onScreen && visibilityAnimated <= 0.02f)
		return;

// #ifdef LIBV_DEV_BUILD
	// The normal culling logic for LensFlare would work perfectly fine without this test,
	// this is only for pedantic debug visualisation with frozen view frustum.
	if (!ctx.testVisibility(matM, BoundingSphere{{}, visibilityTestTolerance}))
		visibilityTestUV = libv::vec2f{-1, -1}; // UV (-1, -1) is implicitly off-screen, this way the animation is kept
// #endif

	// Prepare mesh and UBlock data
	if (dirty) {
		dirty = false;

		struct VertexShadeInfo {
			libv::vec3f position = libv::uninitialized;
			libv::vec2f uv = libv::uninitialized;
			libv::vec4f color = libv::uninitialized;
			libv::vec4f custom1 = libv::uninitialized;
			libv::vec4f custom0 = libv::uninitialized;
		};

		const auto numVertices = elements.size() * 4;
		const auto numIndices = elements.size() * 3 * 2;
		auto vertex = mesh->addBuffer<VertexShadeInfo>(libv::gl::BufferUsage::StaticDraw, numVertices);
		mesh->addAttribute(vertex, attrPosition, &VertexShadeInfo::position);
		mesh->addAttribute(vertex, attrUV, &VertexShadeInfo::uv);
		mesh->addAttribute(vertex, attrDiffuse, &VertexShadeInfo::color);
		mesh->addAttribute(vertex, attrCustom1, &VertexShadeInfo::custom1);
		mesh->addAttribute(vertex, attrCustom0, &VertexShadeInfo::custom0);
		auto index = mesh->addIndex<uint32_t>(libv::gl::BufferUsage::StaticDraw, numIndices);

		for (uint32_t i = 0; i < static_cast<uint32_t>(elements.size()); ++i) {
			const auto& element = elements[i];
			vertex[i * 4 + 0].position = libv::vec3f(-0.5f, -0.5f, 0);
			vertex[i * 4 + 1].position = libv::vec3f(+0.5f, -0.5f, 0);
			vertex[i * 4 + 2].position = libv::vec3f(+0.5f, +0.5f, 0);
			vertex[i * 4 + 3].position = libv::vec3f(-0.5f, +0.5f, 0);
			vertex[i * 4 + 0].uv = libv::vec2f(0, 0);
			vertex[i * 4 + 1].uv = libv::vec2f(1, 0);
			vertex[i * 4 + 2].uv = libv::vec2f(1, 1);
			vertex[i * 4 + 3].uv = libv::vec2f(0, 1);
			vertex[i * 4 + 0].color =
			vertex[i * 4 + 1].color =
			vertex[i * 4 + 2].color =
			vertex[i * 4 + 3].color = element.color;
			vertex[i * 4 + 0].custom1 =
			vertex[i * 4 + 1].custom1 =
			vertex[i * 4 + 2].custom1 =
			vertex[i * 4 + 3].custom1 = libv::vec4f{element.position, element.rotation, element.rotateToCenter, element.colorLightAttenuation};
			vertex[i * 4 + 0].custom0 =
			vertex[i * 4 + 1].custom0 =
			vertex[i * 4 + 2].custom0 =
			vertex[i * 4 + 3].custom0 = libv::vec4f{element.size, element.sizeRadialEdge, 0, 0};
			index[i * 6 + 0] = i * 4 + 0;
			index[i * 6 + 1] = i * 4 + 1;
			index[i * 6 + 2] = i * 4 + 2;
			index[i * 6 + 3] = i * 4 + 2;
			index[i * 6 + 4] = i * 4 + 3;
			index[i * 6 + 5] = i * 4 + 0;
		}
	}

	const auto lightColor = isLight ? static_cast<const Light&>(*target).color : libv::vec3f{1};
	materialBlock->targetPositionW = targetPositionW;
	materialBlock->lightColor = lightColor;

	visibilityUpdated = false;

	// Actual queue
	for (uint32_t i = 0; i < static_cast<uint32_t>(elements.size()); ++i)
		ctx.queue(this, matM, mesh.get(), elements[i].material.get(), SubObjectIndex{i});
}

void LensFlare::queueOverride(ContextQueue& ctx, ContextQueueOverride& ov) {
	// Override is not supported for lens flare
	(void) ctx;
	(void) ov;
}

void LensFlare::gl_update(ContextUpdate& ctx) {
	(void) ctx;
}

void LensFlare::gl_render(ContextRender& ctx, SubObjectIndex subObjectIndex) {
	// Due to depth query management, the visibility animation happens in the first flare render call
	if (!std::exchange(visibilityUpdated, true)) {
		int visibleSamples = 0;
		const auto onScreen = libv::vec::within(visibilityTestUV, {0, 0}, {1, 1});
		if (onScreen) {
			const auto results = ctx.gl_depthQueryResult(ctx.gl, visibilityDepthQuery, 13);
			// Results can be empty
			for (const auto depth : results) {
				const auto world = ctx.frame.screenPicker.world_from_uvd(visibilityTestUV, depth.x);
				const auto dist = length(ctx.frame.eyeW - world);
				if (std::isinf(dist) || visibilityDepthDistance - visibilityTestTolerance <= dist)
					++visibleSamples;
			}
		}

		const auto currentVisibility = static_cast<float>(visibleSamples) / 13.f;
		const auto animationSpeed = visibilityFadeTime.count() != 0 ? ctx.frame.deltaTimeRealf.count() / visibilityFadeTime.count() : 0;
		visibilityAnimated = libv::mix_toward(visibilityAnimated, currentVisibility, animationSpeed);

		for (const auto& element : elements)
			element.material->visibility = visibilityAnimated;
	}

	if (visibilityAnimated <= 0.02f)
		return;

	assert(+subObjectIndex < elements.size());
	mesh->gl_render(ctx, +subObjectIndex * 6, 6);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
