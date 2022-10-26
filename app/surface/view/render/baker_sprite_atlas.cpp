// Project: libv, File: app/surface/view/render/baker_sprite_atlas.cpp

// hpp
#include <surface/view/render/baker_sprite_atlas.hpp>
// libv
#include <libv/glr/queue.hpp>
#include <libv/rev/materials/material_sprite_baker.hpp>
#include <libv/rev/model.hpp>
#include <libv/rev/resource_manager.hpp>
#include <libv/utility/min_max.hpp>
//#include <libv/glr/procedural/cube.hpp>

//#include <surface/view/render/renderer.hpp>
//// pro
//#include <surface/log.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

BakerSpriteAtlas::BakerSpriteAtlas(libv::rev::ResourceManager& loader, libv::vec2i atlasSize, int msaaSamples, int ssaaSamples) :
	atlasSize (atlasSize),
	ssaaSamples(static_cast<int32_t>(std::sqrt(ssaaSamples))),
	bakeSize(atlasSize * this->ssaaSamples),
	loader(loader),
	shaderSpriteBakerDownsample(loader.shader, "builtin/full_screen.vs", "builtin/sprite_baker_downsample.fs") {

	assert(ssaaSamples == 1 || ssaaSamples == 4 || ssaaSamples == 9 || ssaaSamples == 16);

	bakeMSColor.storage_ms(bakeSize, msaaSamples, false);
	bakeMSNormal.storage_ms(bakeSize, msaaSamples, false);
	bakeMSDepth.storage_ms(bakeSize, msaaSamples, false);

	bakeMSFBO.attach2D(libv::gl::Attachment::Color0, bakeMSColor);
	bakeMSFBO.attach2D(libv::gl::Attachment::Color1, bakeMSNormal);
	bakeMSFBO.attach2D(libv::gl::Attachment::Depth, bakeMSDepth);

	bakeSSColor.storage(1, bakeSize);
	bakeSSNormal.storage(1, bakeSize);
	bakeSSDepth.storage(1, bakeSize);

	bakeSSFBO.attach2D(libv::gl::Attachment::Color0, bakeSSColor);
	bakeSSFBO.attach2D(libv::gl::Attachment::Color1, bakeSSNormal);
	bakeSSFBO.attach2D(libv::gl::Attachment::Depth, bakeSSDepth);

//	log_surface.info("Bake Size {}", bakeSize);
}

void BakerSpriteAtlas::renderAtlas(SpriteDefinition& def, libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const libv::rev::Model& model, float sizeMultiplier) {
	// Model is forced to be loaded at this point, so accessing model data is allowed
	const auto AABB_max = model.AABB_max();
	const auto AABB_min = model.AABB_min();

	// Tree
	auto AABB_extent = libv::max(libv::abs(AABB_max), libv::abs(AABB_min)) * 2.0f;
	// Z empty space is adjusted with offset
	AABB_extent.z = std::abs(AABB_max.z - AABB_min.z);

	const auto bake_tile_size = def.tile_size * ssaaSamples;
	const auto bake_tile_sizef = bake_tile_size.cast<float>();

	// NOTE: This scale will cause some clipping in rotated views, but its good enough for now
	const auto bake_scale = libv::min(
			static_cast<float>(bake_tile_size.x) / AABB_extent.x,
			static_cast<float>(bake_tile_size.x) / AABB_extent.y,
			static_cast<float>(bake_tile_size.x) / AABB_extent.z,
			static_cast<float>(bake_tile_size.y) / AABB_extent.x,
			static_cast<float>(bake_tile_size.y) / AABB_extent.y,
			static_cast<float>(bake_tile_size.y) / AABB_extent.z
	);
	def.model_offset = libv::vec3f{0, 0, (AABB_max.z + AABB_min.z) * 0.5f};

	const auto angle_x_range = def.angle_x_max - def.angle_x_min;
	const auto angle_y_range = def.angle_y_max - def.angle_y_min;
	const auto angle_x_step = angle_x_range / static_cast<float>(def.tile_num_x);
	const auto angle_y_step = angle_y_range / static_cast<float>(std::max(def.tile_num_y - 1, 1));

	const auto guard_vp = glr.viewport_guard();
	const auto guard_p = glr.projection.push_guard();
	const auto guard_v = glr.view.push_guard();
	const auto guard_m = glr.model.push_guard();

	glr.projection = libv::mat4f::ortho(
			-0.5f * bake_tile_sizef.x,
			+0.5f * bake_tile_sizef.x,
			-0.5f * bake_tile_sizef.y,
			+0.5f * bake_tile_sizef.y,
//				-1000.0f,
//				+1000.0f);
//				-2.0f * def.model_scale,
//				+2.0f * def.model_scale);
			-1.0f * bake_tile_sizef.y,
			+1.0f * bake_tile_sizef.y);

	glr.view = libv::mat4f::lookAt(libv::vec3f(1, 0, 0), libv::vec3f::zero(), libv::vec3f(0, 0, 1));

	glr.model.scale(bake_scale);
	glr.model.translate(-def.model_offset);

	for (int y = 0; y < def.tile_num_y; ++y) {
		const auto yf = static_cast<float>(y);

		for (int x = 0; x < def.tile_num_x; ++x) {
			const auto xf = static_cast<float>(x);

			const auto guard_v2 = glr.view.push_guard();
			glr.view.rotate(libv::radian(yf * angle_y_step), 0, 1, 0);
			glr.view.rotate(libv::radian(xf * angle_x_step), 0, 0, 1);

			glr.viewport(libv::vec2i{x, y} * bake_tile_size, bake_tile_size);

//				renderSprite(glr);
			model.render(glr, uniform_stream);
		}
	}

	const auto model_scale = libv::max(AABB_extent.x, AABB_extent.y, AABB_extent.z);

	def.model_scale = model_scale * sizeMultiplier;
	def.model_offset *= sizeMultiplier;
}

void BakerSpriteAtlas::bake(SpriteAtlas& atlas, uint32_t index,
		libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream,
		std::string_view modelName, float sizeMultiplier) {

	// --- Fetch model ---

	auto model = loader.model.load(modelName, libv::rev::MaterialSpriteBaker::create, 100);
	loader.update(glr.out_of_order_gl()); // Force model loading

	// --- Setup ---

	auto guard_vp = glr.viewport_guard({0, 0}, atlasSize);
	auto guard_m = glr.model.push_guard();
	auto guard_s = glr.state.push_guard();

	glr.state.enableBlend();
	glr.state.blendSrc_SourceAlpha();
	glr.state.blendDst_One_Minus_SourceAlpha();

	glr.state.disableCullFace(); // To handle two-sided tree models
	glr.state.frontFaceCCW();

	glr.state.enableDepthTest();
	glr.state.depthFunctionLess();

	glr.framebuffer_draw(bakeMSFBO);
	glr.setClearColor(0.5f, 0.5f, 0.5f, 0);
	glr.clearColorDepth();

	// --- Render ---

	renderAtlas(atlas.definitions[index], glr, uniform_stream, model, sizeMultiplier);

	// --- Resolve ---

	glr.blit(bakeMSFBO, bakeSSFBO,
			libv::gl::Attachment::Color0, {0, 0}, bakeSize,
			libv::gl::Attachment::Color0, {0, 0}, bakeSize,
			libv::gl::BufferBit::ColorDepth, libv::gl::MagFilter::Nearest);

	glr.blit(bakeMSFBO, bakeSSFBO,
			libv::gl::Attachment::Color1, {0, 0}, bakeSize,
			libv::gl::Attachment::Color1, {0, 0}, bakeSize,
			libv::gl::BufferBit::Color, libv::gl::MagFilter::Nearest);

	// --- Debug display ---

//		// Blit to default just to debug
//		glr.blit_to_default(result_fbo,
//				libv::gl::Attachment::Color0, {0, 0}, bakeSize or atlasSize,
//				{0, 0}, bakeSize or atlasSize,
//				libv::gl::BufferBit::Color, libv::gl::MagFilter::Nearest);

	// --- Upsample ---

	// GLR doesn't support temporal flux on resources, so 2 temporary framebuffer has to be created for each sprite atlas
	libv::glr::Framebuffer resultFBO;

	resultFBO.attach2D(libv::gl::Attachment::Color0, atlas.textureColor, 0, index);
	resultFBO.attach2D(libv::gl::Attachment::Color1, atlas.textureNormal, 0, index);

	glr.framebuffer_draw(resultFBO);
	// No need to clear, depth test or blend: full screen quad will override everything
	glr.state.disableDepthTest();
	glr.state.disableDepthMask();
	glr.state.disableBlend();

	{
		glr.program(shaderSpriteBakerDownsample.program());
		glr.uniform(shaderSpriteBakerDownsample.uniform().ssaaSamples, ssaaSamples);
		glr.texture(bakeSSColor, textureChannel_diffuse);
		glr.texture(bakeSSNormal, textureChannel_normal);
		glr.render_full_screen();
	}

	glr.framebuffer_default();
}

// -------------------------------------------------------------------------------------------------

} // namespace surface
