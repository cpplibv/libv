// Project: libv.rev, File: sandbox/libv_rev_res_main.cpp

// hpp
#include "libv_gl_runner.hpp"
// libv
#include <libv/math/angle.hpp>
#include <libv/utility/chrono.hpp>
#include <libv/utility/nexus.hpp>
#include <libv/utility/read_file.hpp>
//#include <libv/gl/gl.hpp>
// std
#include <chrono>
#include <iostream>
// pro
#include <libv/gl/framebuffer.hpp>
#include <libv/gl/gl.hpp>
#include <libv/glr/attribute.hpp>
#include <libv/glr/framebuffer.hpp>
#include <libv/glr/layout_std140.hpp>
#include <libv/glr/layout_to_string.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/procedural/ignore.hpp>
#include <libv/glr/procedural/sphere.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/remote.hpp>
#include <libv/glr/uniform_buffer.hpp>
#include <libv/img/save.hpp>
#include <libv/rev/shader/block/matrices.hpp>
#include <libv/rev/materials/material_sprite_baker.hpp>
#include <libv/rev/model.hpp>
#include <libv/rev/post_processing.hpp>
#include <libv/rev/render_target.hpp>
#include <libv/rev/renderer/renderer_editor_grid.hpp>
#include <libv/rev/resource/material_scanner.hpp>
#include <libv/rev/resource/shader.hpp>
#include <libv/rev/resource/shader_loader.hpp>
#include <libv/rev/resource_manager.hpp>
#include <libv/rev/settings.hpp>
#include <libv/rev/shader/attribute.hpp>
#include <libv/rev/texture.hpp>
#include <libv/utility/min_max.hpp>
#include <libv/utility/write_file.hpp>
#include <libv/vm4/load.hpp>
//#include <libv/gl/enum.hpp>
//#include <libv/gl/image.hpp>
//#include <libv/glr/attribute.hpp>
//#include <libv/glr/font.hpp>
//#include <libv/glr/procedural/cube.hpp>
//#include <libv/glr/procedural/progress_segmented_ring.hpp>
//#include <libv/glr/program.hpp>
//#include <libv/glr/text.hpp>
//#include <libv/glr/texture.hpp>
//#include <libv/glr/uniform.hpp>
//#include <libv/glr/uniform_block_binding.hpp>
//#include <libv/rev/resource/texture_loader.hpp>


// -------------------------------------------------------------------------------------------------

//constexpr uint32_t WINDOW_WIDTH = 1280;
//constexpr uint32_t WINDOW_HEIGHT = 800;
constexpr uint32_t WINDOW_WIDTH = 1024;
constexpr uint32_t WINDOW_HEIGHT = 1024;

// -------------------------------------------------------------------------------------------------

struct UniformLayoutSpriteDefinition {
	static inline auto name() { return "SpriteDefinition"; }

	// Ordering matters: Block is packed to 48 Byte

	libv::glr::Uniform_vec2i tile_size;
	libv::glr::Uniform_int32 tile_num_x;
	libv::glr::Uniform_int32 tile_num_y;

	libv::glr::Uniform_vec3f model_offset;
	libv::glr::Uniform_float model_scale;

	libv::glr::Uniform_float angle_x_min;
	libv::glr::Uniform_float angle_x_max;
	libv::glr::Uniform_float angle_y_min;
	libv::glr::Uniform_float angle_y_max;

	LIBV_REFLECTION_ACCESS(tile_size);
	LIBV_REFLECTION_ACCESS(tile_num_x);
	LIBV_REFLECTION_ACCESS(tile_num_y);

	LIBV_REFLECTION_ACCESS(model_offset);
	LIBV_REFLECTION_ACCESS(model_scale);

	LIBV_REFLECTION_ACCESS(angle_x_min);
	LIBV_REFLECTION_ACCESS(angle_x_max);
	LIBV_REFLECTION_ACCESS(angle_y_min);
	LIBV_REFLECTION_ACCESS(angle_y_max);
};

struct UniformLayoutSpriteDefinitions {
	// With 48 byte as SpriteDefinition size the maximum sprite definition count is: 65536 / 48 = 1365
	// std::array<UniformLayoutSpriteDefinition, 65536 / 48> spriteDefinitions;
	std::array<UniformLayoutSpriteDefinition, 64> spriteDefinitions;

	LIBV_REFLECTION_ACCESS(spriteDefinitions);
};

const auto uniformBlock_definitions = libv::glr::UniformBlockBinding{1, "SpriteDefinitions"};

const auto layout_definitions = libv::glr::layout_std140<UniformLayoutSpriteDefinitions>(uniformBlock_definitions);

// -------------------------------------------------------------------------------------------------

struct UniformsSprite {
	libv::glr::Uniform_texture textureColor;
	libv::glr::Uniform_texture textureNormal;

	template <typename Access> void access_uniforms(Access& access) {
		access(textureColor, "textureColor", libv::rev::textureChannel_diffuse);
		access(textureNormal, "textureNormal", libv::rev::textureChannel_normal);
	}

	template <typename Access> void access_blocks(Access& access) {
		access(libv::rev::uniformBlock_matrices);
		access(uniformBlock_definitions);
	}
};

using ShaderSprite = libv::rev::Shader<UniformsSprite>;

// -------------------------------------------------------------------------------------------------

struct SpriteDefinition {
	libv::vec2i tile_size = {64, 64};

	float model_scale = 1.f; // Billboard size
	libv::vec3f model_offset = {0.f, 0.f, 0.f}; // Billboard offset compared to origin

	int tile_num_x = 16;
	int tile_num_y = 8;
	float angle_x_min = 0.f;
	float angle_x_max = libv::tau * (static_cast<float>(tile_num_y) - 1) / static_cast<float>(tile_num_y);
	float angle_y_min = 0.f;
	float angle_y_max = libv::pi / 2.f;
};

struct Sprite {
	SpriteDefinition definition;
	libv::glr::Texture2D::R8_G8_B8_A8 texture_color;
	libv::glr::Texture2D::R8_G8_B8 texture_normal;
};

class RendererSprite {
private:
	struct Entry {
		libv::vec3f position;
		//libv::vec3f normal;
		//float rotation;
		//float height or scale;
		//libv::vec3f hsv_shift;
		//int id;
	};

public:
	ShaderSprite shader;
	std::shared_ptr<Sprite> sprite;
	libv::glr::Mesh mesh{libv::gl::Primitive::Points, libv::gl::BufferUsage::StaticDraw};
	libv::glr::UniformBlockSharedView_std140 spriteDefinitionsBlock;

private:
	std::vector<Entry> entries;
	bool dirty = false;
	bool active = false;

public:
	explicit RendererSprite(libv::rev::ResourceManager& loader) :
		shader(loader.shader, "builtin/sprite.vs", "builtin/sprite.gs", "builtin/sprite.fs") {}

public:
	void add(libv::vec3f position) {
		entries.emplace_back(position);
		dirty = true;
	}

private:
	void updateMesh(libv::glr::UniformBuffer& uniform_stream) {
		dirty = false;
		active = !entries.empty();

		mesh.clear();

		if (!active)
			return;

//		std::cout << libv::glr::layout_to_string<UniformLayoutSpriteDefinitions>("SpriteDefinitions") << std::endl;
//		std::cout << libv::glr::layout_std140_size<UniformLayoutSpriteDefinitions>() << std::endl;

		// --- Mesh

		auto position = mesh.attribute(libv::rev::attribute_position);
//		auto normal = mesh.attribute(libv::rev::attribute_normal);

		auto index = mesh.index();

		position.reserve(entries.size());
		for (const auto& vertex : entries)
			position(vertex.position);

//		normal.reserve(entries.size());
//		for (const auto& vertex : entries)
//			normal(vertex.normal);

//		auto position = mesh.attribute();

		for (uint32_t i = 0; i < entries.size(); ++i)
			index(i);

		// --- Uniform Sprite Definitions

		spriteDefinitionsBlock = uniform_stream.block_shared(layout_definitions);
		spriteDefinitionsBlock[layout_definitions.spriteDefinitions[0].model_scale] = 1.0f;
		spriteDefinitionsBlock[layout_definitions.spriteDefinitions[1].model_scale] = 2.0f;
//		spriteDefinitionsBlock[layout_definitions.spriteDefinitions[0].model_scale] = 1.0f;
//		spriteDefinitionsBlock[layout_definitions.spriteDefinitions[0].model_scale] = 1.0f;
//		spriteDefinitionsBlock[layout_definitions.spriteDefinitions[0].model_scale] = 1.0f;
//		spriteDefinitionsBlock[layout_definitions.spriteDefinitions[0].model_scale] = 1.0f;
//		spriteDefinitionsBlock[layout_definitions.spriteDefinitions[0].model_scale] = 1.0f;
//		...
	}

public:
	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
		if (dirty)
			updateMesh(uniform_stream);

		if (!active)
			return;

		glr.program(shader.program());

		auto block_matrices = uniform_stream.block_shared(libv::rev::layout_matrices);
		block_matrices[libv::rev::layout_matrices.matMVP] = glr.mvp();
		block_matrices[libv::rev::layout_matrices.matM] = glr.model;
		block_matrices[libv::rev::layout_matrices.matP] = glr.projection;
		block_matrices[libv::rev::layout_matrices.eye] = glr.eye();
		glr.uniform(std::move(block_matrices));
		glr.uniform(spriteDefinitionsBlock);
		glr.texture(sprite->texture_color, libv::rev::textureChannel_diffuse);
		glr.texture(sprite->texture_normal, libv::rev::textureChannel_normal);
		glr.render(mesh);
	}
};

// -------------------------------------------------------------------------------------------------

struct UniformsSpriteBakerDownsample {
	libv::glr::Uniform_texture texture0;
	libv::glr::Uniform_int32 ssaaSamples;
	libv::glr::Uniform_bool isColor;

	template <typename Access> void access_uniforms(Access& access) {
		access(texture0, "texture0", libv::rev::textureChannel_diffuse);
		access(ssaaSamples, "ssaaSamples");
		access(isColor, "isColor");
	}

	template <typename Access> void access_blocks(Access& access) {
		access(libv::rev::uniformBlock_matrices);
	}
};

using ShaderSpriteBakerDownsample = libv::rev::Shader<UniformsSpriteBakerDownsample>;

// -------------------------------------------------------------------------------------------------

struct SpriteAtlasBaker {
	libv::vec2i atlasSize;
	int32_t ssaaSamples;

	libv::vec2i bakeSize;
	libv::rev::ResourceManager& loader;

	libv::glr::Framebuffer bakeMSFBO;
	libv::glr::Texture2DMultisample::R8_G8_B8_A8 bakeMSColor;
	libv::glr::Texture2DMultisample::R8_G8_B8 bakeMSNormal;
	libv::glr::Texture2DMultisample::D16 bakeMSDepth;

	libv::glr::Framebuffer bakeSSFBO;
	libv::glr::Texture2D::R8_G8_B8_A8 bakeSSColor;
	libv::glr::Texture2D::R8_G8_B8 bakeSSNormal;
	libv::glr::Texture2D::D16 bakeSSDepth;

	libv::glr::Framebuffer resultFBOColor;

	libv::glr::Framebuffer resultFBONormal;

	ShaderSpriteBakerDownsample shaderSpriteBakerDownsample;

//	SpriteAtlas result{atlasSize};
//	libv::glr::Framebuffer result_fbo;

	explicit SpriteAtlasBaker(libv::rev::ResourceManager& loader, libv::vec2i atlasSize, int msaaSamples = 16, int ssaaSamples = 16) :
		atlasSize(atlasSize),
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
	}

	SpriteDefinition renderAtlas(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const libv::rev::Model& model) {
		const auto AABB_max = libv::vec3f{178.889f, 184.191f, 382.196f} * 0.175f;
		const auto AABB_min = libv::vec3f{-184.492f, -170.332f, 0.f} * 0.175f;
		// load vm4
//		libv::rev::Model model(vm4, materialLoader, libv::rev::SpriteBaker::create);

		SpriteDefinition def;
//		def.tile_size.x = 10;
//		def.tile_size.x = 64;
//		def.tile_size.x = 256;
//		def.tile_size.y = 256;

		// Tree
		auto AABB_extent = libv::max(libv::abs(AABB_max), libv::abs(AABB_min)) * 2.0f;
		// Z empty space is adjusted with offset
		AABB_extent.z = std::abs(AABB_max.z - AABB_min.z);

		const auto bake_tile_size = def.tile_size * ssaaSamples;
		const auto bake_tile_sizef = bake_tile_size.cast<float>();

		// NOTE: This scale will cause some clipping in rotated views, but its good enough for now
		def.model_scale = libv::min(
				static_cast<float>(bake_tile_size.x) / AABB_extent.x,
				static_cast<float>(bake_tile_size.x) / AABB_extent.y,
				static_cast<float>(bake_tile_size.x) / AABB_extent.z,
				static_cast<float>(bake_tile_size.y) / AABB_extent.x,
				static_cast<float>(bake_tile_size.y) / AABB_extent.y,
				static_cast<float>(bake_tile_size.y) / AABB_extent.z
		);
		def.model_offset = libv::vec3f{0, 0, (AABB_max.z + AABB_min.z) * -0.5f};

		const auto angle_x_range = def.angle_x_max - def.angle_x_min;
		const auto angle_y_range = def.angle_y_max - def.angle_y_min;
		const auto angle_x_step = angle_x_range / static_cast<float>(def.tile_num_x - 1);
		const auto angle_y_step = angle_y_range / static_cast<float>(def.tile_num_y - 1);

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

		glr.model.scale(def.model_scale);
		glr.model.translate(def.model_offset);

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

		return def;
	}

	std::shared_ptr<Sprite> bake(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, std::string_view modelName) {
		// --- Fetch model ---

		auto model = loader.model.load(modelName, libv::rev::MaterialSpriteBaker::create, 100);
		loader.update(glr.out_of_order_gl()); // Force model loading

		// --- Setup ---

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
//		glr.setClearColor(1, 1, 1, 1);
		glr.setClearColor(0, 0, 0, 0);
		glr.clearColor();
		glr.clearDepth();

		// --- Render ---

		auto def = renderAtlas(glr, uniform_stream, model);

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

//		libv::glr::Texture2DArray::R8_G8_B8_A8 resultColor;
		libv::glr::Texture2D::R8_G8_B8_A8 resultColor;
		resultColor.storage(1, atlasSize);
		resultColor.set(libv::gl::MagFilter::Linear);
		resultColor.set(libv::gl::MinFilter::Linear);
		resultFBOColor.attach2D(libv::gl::Attachment::Color0, resultColor);
//		resultFBOColor.attach3D(libv::gl::Attachment::Color0, resultColor, 0, ID);

//		libv::glr::Texture2DArray::R8_G8_B8 resultNormal;
		libv::glr::Texture2D::R8_G8_B8 resultNormal;
		resultNormal.storage(1, atlasSize);
		resultNormal.set(libv::gl::MagFilter::Linear);
		resultNormal.set(libv::gl::MinFilter::Linear);
//		resultNormal.set(libv::gl::Wrap::ClampToEdge, libv::gl::Wrap::ClampToEdge);
		resultFBONormal.attach2D(libv::gl::Attachment::Color0, resultNormal);
//		resultFBONormal.attach2D(libv::gl::Attachment::Color0, resultNormal, 0, ID);

		glr.setClearColor(0, 0, 0, 0);
		glr.state.disableDepthTest();
		glr.state.disableDepthMask();

		{
			glr.framebuffer_draw(resultFBOColor);
			glr.program(shaderSpriteBakerDownsample.program());
			glr.texture(bakeSSColor, libv::rev::textureChannel_diffuse);
			glr.uniform(shaderSpriteBakerDownsample.uniform().ssaaSamples, ssaaSamples);
			glr.uniform(shaderSpriteBakerDownsample.uniform().isColor, true);
			glr.render_full_screen();
		} {
			glr.framebuffer_draw(resultFBONormal);
			glr.program(shaderSpriteBakerDownsample.program());
			glr.texture(bakeSSNormal, libv::rev::textureChannel_diffuse);
			glr.uniform(shaderSpriteBakerDownsample.uniform().ssaaSamples, ssaaSamples);
			glr.uniform(shaderSpriteBakerDownsample.uniform().isColor, false);
			glr.render_full_screen();
		}

		resultColor.generate_mipmaps();
		resultNormal.generate_mipmaps();

		glr.framebuffer_default();

		// --- Retrieve Textures ---

//		glr.callback([size = atlasSize, result = result](libv::gl::GL& gl) {
//		glr.callback([size = bakeSize, result = result](libv::gl::GL& gl) {
//			const auto flipY = []<typename C>(C& data, size_t sizeX) {
//				using value_type = typename C::value_type;
//				const auto numRow = data.size() / sizeX;
//				auto tmpRow = std::make_unique_for_overwrite<value_type[]>(sizeX);
//
//				for (size_t i = 0; i < numRow / 2; ++i) {
//					void* const tmp = tmpRow.get();
//					void* const front = data.data() + i * sizeX;
//					void* const back = data.data() + (numRow - 1 - i) * sizeX;
//
//					std::memcpy(tmp, front, sizeX * sizeof(value_type));
//					std::memcpy(front, back, sizeX * sizeof(value_type));
//					std::memcpy(back, tmp, sizeX * sizeof(value_type));
//				}
//			};
//
//			{
//				std::vector<libv::vec4uc> data;
//				data.resize(size.x * size.y);
//				gl.readTextureImage(result.color.out_of_order_gl(), 0, libv::gl::ReadFormat::RGBA, libv::gl::DataType::U8, data.data());
//				flipY(data, size.x);
//				auto imgData = libv::img::save_png(data.data(), size.x, size.y);
//				libv::write_file_or_throw("test_color.png", imgData.span());
//			} {
//				std::vector<libv::vec3uc> data;
//				data.resize(size.x * size.y);
//				gl.readTextureImage(result.normal.out_of_order_gl(), 0, libv::gl::ReadFormat::RGB, libv::gl::DataType::U8, data.data());
//				flipY(data, size.x);
//				auto imgData = libv::img::save_png(data.data(), size.x, size.y);
//				libv::write_file_or_throw("test_normal.png", imgData.span());
//			} {
//				std::vector<uint16_t> data;
//				data.resize(size.x * size.y);
//				gl.readTextureImage(result.depth.out_of_order_gl(), 0, libv::gl::ReadFormat::DEPTH, libv::gl::DataType::U16, data.data());
//				libv::write_file_or_throw("test_depth.raw", std::as_bytes(std::span(data)));
//			}
//		});

		return std::make_shared<Sprite>(def, resultColor, resultNormal);
	}
};

// =================================================================================================

struct Sandbox {
	std::chrono::duration<float> running_time{0};
	libv::Nexus nexus;

	libv::vec2i window_size = {WINDOW_WIDTH, WINDOW_HEIGHT};

	libv::glr::Remote remote; // Remote has to be the first data member to clean up gl resources

	libv::glr::UniformBuffer uniform_stream{libv::gl::BufferUsage::StreamDraw};

	libv::rev::ResourceManager loader{[] {
		libv::rev::Settings settings;
		settings.texture.base_path = "../../res/texture/";
		settings.shader.base_path = "../../res/shader/";
		settings.model.base_path = "../../res/model/";
		return settings;
	}(), nexus};

	libv::rev::Model keep_alive = loader.model.load("tree_01.vm4", libv::rev::MaterialSpriteBaker::create, 100);

	bool created = false;

	// -------------------------------------------------------------------------------------------------

	RendererSprite sprites{loader};
	libv::rev::RendererEditorGrid grid{loader};

	// -------------------------------------------------------------------------------------------------

	Sandbox() {
		remote.create();
		remote.enableDebug();
	}

	~Sandbox() {
	}

	// -------------------------------------------------------------------------------------------------

	void onResize(int width, int height) {
		window_size.x = width;
		window_size.y = height;

//		render_target.size(window_size);
//		post_processing.size(window_size);

		std::cout << "Resolution: " << width << "x" << height << std::endl;
	}

	void onKey(int key, int scancode, int action, int mods) {
		(void) key;
		(void) scancode;
		(void) mods;

		if (action != GLFW_PRESS)
			return;

//		if (key == GLFW_KEY_GRAVE_ACCENT) {
//			render_target.sampleCount(0); std::cout << "MSAA: Disabled" << std::endl;
//		} else if (key == GLFW_KEY_1) {
//			render_target.sampleCount(2); std::cout << "MSAA: 2" << std::endl;
//		} else if (key == GLFW_KEY_2) {
//			render_target.sampleCount(4); std::cout << "MSAA: 4" << std::endl;
//		} else if (key == GLFW_KEY_3) {
//			render_target.sampleCount(8); std::cout << "MSAA: 8" << std::endl;
//		} else if (key == GLFW_KEY_4) {
//			render_target.sampleCount(16); std::cout << "MSAA: 16" << std::endl;
//		}
	}

	// -------------------------------------------------------------------------------------------------

	void update(const std::chrono::duration<float> deltaTime) {
		running_time = libv::time_mod(running_time + deltaTime, std::chrono::seconds{3600});
	}

	void render() {
		if (!std::exchange(created, true)) {
			auto queue = remote.queue();

			create_remote(queue);

			remote.queue(std::move(queue));
			remote.execute();
		}

		auto queue = remote.queue();

		render_remote(queue);

		remote.queue(std::move(queue));
		remote.execute();
	}

	void create_remote(libv::glr::Queue& glr) {
		SpriteAtlasBaker baker{loader, libv::vec2i{1024, 1024}, 16, 16};

		auto atlas = baker.bake(glr, uniform_stream, "tree_01.vm4");
//		auto atlas2 = baker.bake(glr, uniform_stream, "tree_02.vm4");
//		auto atlas3 = baker.bake(glr, uniform_stream, "tree_03.vm4", -pi, pi, 0, pi/2, 16, 8, 64, 64);

		sprites.sprite = atlas;
		for (int x = -10; x < 10; ++x) {
			for (int y = -10; y < 10; ++y) {
				const auto xf = static_cast<float>(x);
				const auto yf = static_cast<float>(y);
				sprites.add({xf, yf, 0});
			}
		}
	}

	void render_remote(libv::glr::Queue& glr) {
		loader.update(glr.out_of_order_gl());

		const auto guard_s = glr.state.push_guard();
		const auto guard_m = glr.model.push_guard();
		const auto guard_v = glr.view.push_guard();
		const auto guard_p = glr.projection.push_guard();

		glr.state.enableBlend();
		glr.state.blendSrc_SourceAlpha();
		glr.state.blendDst_One_Minus_SourceAlpha();

		glr.state.enableCullFace();
//		glr.state.disableCullFace();
		glr.state.frontFaceCCW();
		glr.state.cullBackFace();

		glr.state.polygonModeFill();
//		glr.state.polygonModeLine();

		glr.state.enableDepthTest();
		glr.state.enableDepthMask();
		glr.state.depthFunctionLess();

		glr.setClearColor(0.098f, 0.2f, 0.298f, 1.0f);
//		glr.setClearColor(1.f, 1.f, 1.f, 1.0f);

		glr.clearColorDepth();

		static constexpr libv::mat4f base_camera_orientation = libv::mat4f(
				 0,  0, -1,  0, // Map Z- to X+
				-1,  0,  0,  0, // Map X+ to Y-
				 0,  1,  0,  0, // Map Y+ to Z+
				 0,  0,  0,  1  //
		);

		glr.projection = libv::mat4f::perspective(libv::deg_to_rad(60.f), static_cast<float>(window_size.x) / static_cast<float>(window_size.y), 0.1f, 10000.f);
//		glr.view = libv::mat4f::lookAt({6, 6, 4}, {0, 0, 0}, {0, 0, 1});
//		glr.view = libv::mat4f::lookAt({6, 6, std::abs(std::fmod(running_time.count() / 3.f, 1.0f)) * 20.f}, {0, 0, 0}, {0, 0, 1});

//		glr.projection = libv::mat4f::ortho(-10, 10, -10, 10, -1000, +1000);
//		glr.view = base_camera_orientation;
		glr.model = libv::mat4f::identity();

//		glr.view.rotate(libv::degrees(running_time.count() * 5.f), 0, 0, 1);

		glr.view = base_camera_orientation;
//		glr.view.rotate(libv::degrees(running_time.count() * 5.f), -0.7, -0.7, 0);
		glr.view.translate(10, 0, 0);

		glr.view.rotate(libv::degrees(std::fmod(running_time.count() * 5.f, 170.f)), 0, -1, 0);
		glr.view.rotate(libv::degrees(30.f), 0, 0, 1);

		// glr.view.rotate(libv::degrees(30.f), 0, -1, 0);
		// glr.view.rotate(libv::degrees(std::fmod(running_time.count() * 5.f, 170.f)), 0, 0, 1);

		// --- Render ---

		grid.render(glr, uniform_stream);
		sprites.render(glr, uniform_stream);
	}
};

// Runner ------------------------------------------------------------------------------------------

int main() {
	std::cout << libv::logger_stream;
	return run_sandbox<Sandbox>("Sandbox libv.rev", WINDOW_WIDTH, WINDOW_HEIGHT, 4, 32);
}
