// Project: libv.rev, File: sandbox/libv_rev_res_main.cpp

// hpp
#include "libv_gl_runner.hpp"
// libv
//#include <libv/gl/gl.hpp>
#include <libv/math/angle.hpp>
#include <libv/utility/chrono.hpp>
#include <libv/utility/read_file.hpp>
// std
#include <chrono>
#include <iostream>
// pro
#include <libv/glr/attribute.hpp>
#include <libv/glr/layout_std140.hpp>
#include <libv/glr/mesh.hpp>

//#include <libv/glr/program.hpp>
#include <libv/gl/gl.hpp>

#include <libv/glr/framebuffer.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/remote.hpp>
#include <libv/glr/uniform_buffer.hpp>

#include <libv/glr/procedural/sphere.hpp>
#include <libv/glr/procedural/ignore.hpp>

#include <libv/rev/post_processing.hpp>
#include <libv/rev/render_target.hpp>
#include <libv/rev/resource/shader.hpp>
#include <libv/rev/resource/shader_loader.hpp>

#include <libv/rev/model.hpp>
#include <libv/rev/resource_manager.hpp>
#include <libv/rev/settings.hpp>
#include <libv/rev/texture.hpp>
//#include <libv/rev/resource/texture_loader.hpp>





#include <libv/rev/resource/material_scanner.hpp>
#include <libv/rev/materials/material_sprite_baker.hpp>
#include <libv/vm4/load.hpp>

//#include <libv/gl/enum.hpp>
//#include <libv/gl/image.hpp>
//#include <libv/glr/attribute.hpp>
//#include <libv/glr/font.hpp>
//#include <libv/glr/procedural/cube.hpp>
//#include <libv/glr/procedural/progress_segmented_ring.hpp>
//#include <libv/glr/text.hpp>
//#include <libv/glr/texture.hpp>
//#include <libv/glr/uniform.hpp>
//#include <libv/glr/uniform_block_binding.hpp>


// -------------------------------------------------------------------------------------------------

constexpr uint32_t WINDOW_WIDTH = 1280;
constexpr uint32_t WINDOW_HEIGHT = 800;

constexpr auto attribute_position  = libv::glr::Attribute<0, libv::vec3f>{};
constexpr auto attribute_color0    = libv::glr::Attribute<2, libv::vec4f>{};
constexpr auto attribute_texture0  = libv::glr::Attribute<8, libv::vec2f>{};

const auto uniformBlock_matrices   = libv::glr::UniformBlockBinding{0, "Matrices"};

constexpr auto textureChannel_diffuse = libv::gl::TextureChannel{0};

// -------------------------------------------------------------------------------------------------

struct UniformLayoutMatrices {
	libv::glr::Uniform_mat4f matMVP;
//	libv::glr::Uniform_mat4f matP;
	libv::glr::Uniform_mat4f matM;
//	libv::glr::Uniform_vec3f color;

	LIBV_REFLECTION_ACCESS(matMVP);
//	LIBV_REFLECTION_ACCESS(matP);
	LIBV_REFLECTION_ACCESS(matM);
//	LIBV_REFLECTION_ACCESS(color);
};
const auto layout_matrices = libv::glr::layout_std140<UniformLayoutMatrices>(uniformBlock_matrices);

// -------------------------------------------------------------------------------------------------

struct UniformsSphere {
//	libv::glr::Uniform_mat4f matMVP;
//	libv::glr::Uniform_mat4f matM;
//	libv::glr::Uniform_vec3f color;
	libv::glr::Uniform_texture texture0;

	template <typename Access> void access_uniforms(Access& access) {
//		access(matMVP, "matMVP");
//		access(render_resolution, "render_resolution");
//		access(time, "time", 0.f);
		access(texture0, "texture0", textureChannel_diffuse);
	}

	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
	}
};

// -------------------------------------------------------------------------------------------------

//struct SpriteAtlas {
//	libv::glr::Texture2D::R8_G8_B8_A8 color;
//	libv::glr::Texture2D::R8_G8_B8 normal;
//	libv::glr::Texture2D::D24 depth;
//};
//
//struct SpriteDefinition {
//	// definition:
//	libv::vec2i tile_size = {64, 64};
//
//	// import only:
//	float model_scale = 1.f;
//	// import and maybe definition:
//	libv::vec3f model_offset = {0.f, 0.f, 0.f};
//
//	// definition:
//	int tile_num_x = 16;
//	int tile_num_y = 8;
//	float angle_x_min = 0.f;
//	float angle_x_max = libv::tau * (static_cast<float>(tile_num_y) - 1) / static_cast<float>(tile_num_y);
//	float angle_y_min = 0.f;
//	float angle_y_max = libv::pi / 2.f;
//};
//
//struct SpriteAtlasBaker {
//	libv::glr::Framebuffer fbo;
//	libv::glr::Texture2DMultisample::R8_G8_B8_A8 color;
//	libv::glr::Texture2DMultisample::R8_G8_B8 normal;
//	libv::glr::Texture2DMultisample::D24 depth;
//
////	libv::rev::RenderTarget render_target;
//
////	SpriteAtlasBaker(libv::vec2i atlas_size, int msaa_samples, int super_sample_resolution) {
//	explicit SpriteAtlasBaker(libv::vec2i atlas_size, int msaa_samples = 16) {
//		color.storage_ms(atlas_size, msaa_samples, false);
//		normal.storage_ms(atlas_size, msaa_samples, false);
//		depth.storage_ms(atlas_size, msaa_samples, false);
//
//		fbo.attach2D(libv::gl::Attachment::Color0, color);
//		fbo.attach2D(libv::gl::Attachment::Color1, normal);
//		fbo.attach2D(libv::gl::Attachment::Depth, depth);
////		render_target.attach(color, libv::glr::attachment0);
////		render_target.attach(normal, libv::glr::attachment1);
////		render_target.attach(depth, libv::glr::attachment2);
//	}
//
//	void render(libv::glr::Queue& glr) {
//
//	}
//
//	SpriteAtlas bake(libv::glr::Queue& glr) {
//		SpriteAtlas result;
//
////		libv::glr::Framebuffer fbo;
////		fbo.attach2D(libv::gl::Attachment::Color0, result.color);
////		fbo.attach2D(libv::gl::Attachment::Color1, result.normal);
////		fbo.attach2D(libv::gl::Attachment::Depth, result.depth);
//
//		glr.blit(render_target.framebuffer(), fbo, {0, 0}, render_target.size(), {0, 0}, dst_size, libv::gl::BufferBit::ColorDepth, libv::gl::MagFilter::Linear);
//
////		libv::rev::RenderTarget result_target(atlas_size, msaa_samples);
////		result_target.attach(result.color, libv::glr::attachment0);
////		result_target.attach(result.normal, libv::glr::attachment1);
////		result_target.attach(result.depth, libv::glr::attachment2);
////
////		glr.blit(render_target.framebuffer(), result_target.framebuffer());
//
//		return result;
//	}
//};

struct Sandbox {
	int debug_view_bloom_step = 0;
	std::chrono::duration<float> running_time{0};

	libv::vec2i window_size = {WINDOW_WIDTH, WINDOW_HEIGHT};

	libv::glr::Remote remote; // Remote has to be the first data member to clean up gl resources

	libv::glr::Mesh mesh_sphere{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	libv::glr::Mesh mesh_color_bar{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

	libv::glr::UniformBuffer uniform_stream{libv::gl::BufferUsage::StreamDraw};

	libv::rev::ResourceManager loader{[] {
		libv::rev::Settings settings;
		settings.texture.base_path = "../res/texture/";
		settings.shader.base_path = "../res/shader/";
		settings.model.base_path = "../res/model/";
		return settings;
	}()};
	libv::rev::Texture texture0 = loader.texture.load("hexagon_metal_0001_diffuse.dds");
//	libv::rev::Texture texture0 = loader.texture.load("builtin:white");
	libv::rev::Texture texture0b = loader.texture.load("hexagon_metal_0001_diffuse.dds");
	libv::rev::Texture texture1 = loader.texture.load("hexagon_metal_0001_normal.dds");
	libv::rev::Texture texture2 = loader.texture.load("hexagon_metal_0001_emission.dds");
	libv::rev::Texture texture3 = loader.texture.load("not_found.dds");
//	libv::rev::Model model0{libv::vm4::load_or_throw(libv::read_file_or_throw("../res/model/Tree_med.fixed.game.vm4")), loader.material.load};
//	libv::rev::Model model0{libv::vm4::load_or_throw(libv::read_file_or_throw("../res/model/Tree_med.fixed.game.vm4")), [this](const auto& m) {
//	libv::rev::Model model0{libv::vm4::load_or_throw(libv::read_file_or_throw("../res/model/test_node_02_med.game.vm4")), [this](const auto& m) {
//	libv::rev::Model model0{libv::vm4::load_or_throw(libv::read_file_or_throw("../res/model/test_node_med.game.vm4")), [this](const auto& m) {
//	libv::rev::Model model0{libv::vm4::load_or_throw(libv::read_file_or_throw("../res/model/test_shadow_curves_02_med.game.vm4")), [this](const auto& m) {
//	libv::rev::Model model0{libv::vm4::load_or_throw(libv::read_file_or_throw("../res/model/tree_01.vm4")), [this](const auto& m) {
//		libv::rev::MaterialScanner ms(loader.shader, loader.texture, m);
//		return libv::rev::MaterialSpriteBaker::create(ms);
//	}};

	libv::rev::Model model0 = loader.model.load("tree_01.vm4", libv::rev::MaterialSpriteBaker::create);
//	libv::rev::Model model0 = loader.model.load("tree_01.vm4");
//	libv::rev::Model model0 = loader.model.fallback();

//	libv::rev::Shader<UniformsSphere> shader_sphere{loader.shader, "rev_sandbox/sphere.vs", "rev_sandbox/sphere.fs"};
	libv::rev::Shader<UniformsSphere> shader_sphere{loader.shader, "rev_sandbox/sphere.vs", "rev_sandbox/sphere_tex.fs"};
	libv::rev::Shader<UniformsSphere> shader_color_bar{loader.shader, "rev_sandbox/sphere.vs", "rev_sandbox/color_bar.fs"};

	libv::rev::RenderTarget render_target{window_size, 4};
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBufferTexture, 0);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_AdditionalColorBufferTexture, 0);
		//uint32_t drawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
		//glDrawBuffers(2, (GLenum*)drawBuffers);

	libv::rev::PostProcessing post_processing{loader.shader, window_size};

	Sandbox() {
		{
			mesh_sphere.clear();

			auto position = mesh_sphere.attribute(attribute_position);
			auto texture0 = mesh_sphere.attribute(attribute_texture0);
			auto index = mesh_sphere.index();

			libv::glr::generateSpherifiedCube(64, position, libv::glr::ignore, texture0, index);
		}
		{
			mesh_color_bar.clear();

			auto position = mesh_color_bar.attribute(attribute_position);
			auto texture0 = mesh_color_bar.attribute(attribute_texture0);
			auto index = mesh_color_bar.index();

			position(-1, -1, 0);
			position(+1, -1, 0);
			position(+1, +1, 0);
			position(-1, +1, 0);

			texture0(0, 0);
			texture0(1, 0);
			texture0(1, 1);
			texture0(0, 1);

			index.quad(0, 1, 2, 3);
		}

		remote.create();
		remote.enableDebug();

		log_sandbox.debug("{:46} [{:>10} ]", "CurrentAvailableVideoMemory",        remote.gl().getCurrentAvailableVideoMemory());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxSamples",                         remote.gl().getMaxSamples());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxSamplesInteger",                  remote.gl().getMaxSamplesInteger());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnits",               remote.gl().getMaxTextureImageUnits());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsCompute",        remote.gl().getMaxTextureImageUnitsCompute());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsFragment",       remote.gl().getMaxTextureImageUnitsFragment());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsGeometry",       remote.gl().getMaxTextureImageUnitsGeometry());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsTessControl",    remote.gl().getMaxTextureImageUnitsTessControl());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsTessEvaluation", remote.gl().getMaxTextureImageUnitsTessEvaluation());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsVertex",         remote.gl().getMaxTextureImageUnitsVertex());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureSize",                     remote.gl().getMaxTextureSize());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxUniformBlockSize",                remote.gl().getMaxUniformBlockSize());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxUniformBufferBindings",           remote.gl().getMaxUniformBufferBindings());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxVertexAttribs",                   remote.gl().getMaxVertexAttribs());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxVertexUniformComponents",         remote.gl().getMaxVertexUniformComponents());
		log_sandbox.debug("{:46} [{:>10} ]", "UniformBufferOffsetAlignment",       remote.gl().getUniformBufferOffsetAlignment());
	}

	~Sandbox() {
	}

	// -------------------------------------------------------------------------------------------------

	void onResize(int width, int height) {
		window_size.x = width;
		window_size.y = height;

		render_target.size(window_size);
		post_processing.size(window_size);

		std::cout << "Resolution: " << width << "x" << height << std::endl;
	}

	void onKey(int key, int scancode, int action, int mods) {
		(void) scancode;
		(void) mods;

		if (action != GLFW_PRESS)
			return;

		if (key == GLFW_KEY_GRAVE_ACCENT) {
			render_target.sampleCount(0); std::cout << "MSAA: Disabled" << std::endl;
		} else if (key == GLFW_KEY_1) {
			render_target.sampleCount(2); std::cout << "MSAA: 2" << std::endl;
		} else if (key == GLFW_KEY_2) {
			render_target.sampleCount(4); std::cout << "MSAA: 4" << std::endl;
		} else if (key == GLFW_KEY_3) {
			render_target.sampleCount(8); std::cout << "MSAA: 8" << std::endl;
		} else if (key == GLFW_KEY_4) {
			render_target.sampleCount(16); std::cout << "MSAA: 16" << std::endl;

		} else if (key == GLFW_KEY_Q) {
			--debug_view_bloom_step; std::cout << "debug_view_bloom_step: " << debug_view_bloom_step << std::endl;
		} else if (key == GLFW_KEY_W) {
			++debug_view_bloom_step; std::cout << "debug_view_bloom_step: " << debug_view_bloom_step << std::endl;
		}
	}

	// -------------------------------------------------------------------------------------------------

	void update(const std::chrono::duration<float> deltaTime) {
		running_time = libv::time_mod(running_time + deltaTime, std::chrono::seconds{3600});
	}

	void render() {
		auto queue = remote.queue();

		render_remote(queue);

		remote.queue(std::move(queue));
		remote.execute();
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

//		glr.state.enableCullFace();
		glr.state.disableCullFace();
		glr.state.frontFaceCCW();
		glr.state.cullBackFace();

		glr.state.polygonModeFill();
//		glr.state.polygonModeLine();

		glr.state.enableDepthTest();
		glr.state.depthFunctionLess();

		glr.setClearColor(0.098f, 0.2f, 0.298f, 1.0f);
//		glr.setClearColor(1.f, 1.f, 1.f, 1.0f);

		static constexpr libv::mat4f base_camera_orientation = libv::mat4f(
				 0,  0, -1,  0, // Map Z- to X+
				-1,  0,  0,  0, // Map X+ to Y-
				 0,  1,  0,  0, // Map Y+ to Z+
				 0,  0,  0,  1  //
		);

//		glr.projection = libv::mat4f::perspective(libv::deg_to_rad(60.f), window_size.x / window_size.y, 0.1f, 10000.f);
//		glr.view = libv::mat4f::lookAt({60, 60, 40}, {0, 0, 0}, {0, 0, 1});

		glr.projection = libv::mat4f::ortho(0, static_cast<float>(window_size.x), 0, static_cast<float>(window_size.y), -1000, +1000);
		glr.view = base_camera_orientation;
		glr.view.rotate(libv::degrees(35.f), 0, -1, 0);
		glr.view.translate(0, static_cast<float>(window_size.x) * -0.5f, 0);
//		glr.view.translate(0, 0, static_cast<float>(window_size.y) * 0.5f);

		glr.view.rotate(libv::degrees(running_time.count() * 5.f), 0, 0, 1);
		glr.model = libv::mat4f::identity();

		// --- Render the Sphere ---

		glr.framebuffer_draw(render_target.framebuffer());
//		glr.framebuffer_draw_default();
		glr.viewport({0, 0}, render_target.size());
		glr.clearColor();
		glr.clearDepth();

		{
			const auto guard_m2 = glr.model.push_guard();
//			glr.model.translate(static_cast<float>(window_size.x) * 0.5f, static_cast<float>(window_size.y) * 0.5f, 0);
//			glr.model.translate(0, static_cast<float>(window_size.x) * 0.5f, 0);
//			glr.model.scale(30.5f);
			glr.model.scale(17.5f);
//			glr.model.scale(12.5f);

//			glr.model.scale(0.175f);

			model0.render(glr, uniform_stream);

//			auto uniforms = uniform_stream.block_unique(layout_matrices);
//			uniforms[layout_matrices.matMVP] = glr.mvp();
//			uniforms[layout_matrices.matM] = glr.model;
//
//			glr.program(shader_sphere.program());
//			glr.texture(texture0.texture(), textureChannel_diffuse);
//			glr.uniform(std::move(uniforms));
//			glr.render(mesh_sphere);
		}

		glr.view = libv::mat4f::identity();
		const auto mini_count = 16;
		const auto mini_countf = static_cast<float>(mini_count);
		for (int i = 0 ; i < mini_count; ++i) {
			const auto fi = static_cast<float>(i);
			const auto guard_m2 = glr.model.push_guard();
			glr.model.translate(static_cast<float>(window_size.x) * 0.9f + std::sin(running_time.count()) * 50.f, (0.5f + fi) / mini_countf * static_cast<float>(window_size.y), 0);
			glr.model.scale(15.f);
			glr.model.rotate(libv::radian(libv::pi * fi / mini_countf), 0, 1, 0); // Using pi and not tau as only half rotation is enough, sphere is symmetric

			auto uniforms = uniform_stream.block_unique(layout_matrices);
			uniforms[layout_matrices.matMVP] = glr.mvp();
			uniforms[layout_matrices.matM] = glr.model;

			glr.program(shader_sphere.program());
			glr.texture(texture0.texture(), textureChannel_diffuse);
			glr.uniform(std::move(uniforms));
			glr.render(mesh_sphere);
		}
//		{
//			// Color-Space Test Bar
//			const auto guard_m2 = glr.model.push_guard();
//			const auto window_size_f = window_size.cast<float>();
//			glr.model.translate(window_size_f.x * 0.5f, window_size_f.y * 0.5f, 500);
//			glr.model.scale(window_size_f.x * 0.5f, window_size_f.y * 0.5f, 0);
//
//			glr.model.translate(0, -0.90f, 0);
//			glr.model.scale(1, 0.10f, 1);
//
//			auto uniforms = uniform_stream.block_unique(layout_matrices);
//			uniforms[layout_matrices.matMVP] = glr.mvp();
//			uniforms[layout_matrices.matM] = glr.model;
//
//			glr.program(shader_color_bar.program());
//			glr.uniform(std::move(uniforms));
//			glr.render(mesh_color_bar);
//		}

		// --- Post Processing ---

		const auto& main_texture = render_target.resolve(glr);
		post_processing.pass(glr, main_texture);
	}
};

// Runner ------------------------------------------------------------------------------------------

int main() {
	std::cout << libv::logger_stream;
	return run_sandbox<Sandbox>("Sandbox libv.rev", WINDOW_HEIGHT, WINDOW_WIDTH, 0, 0);
}
