// Project: libv.glr, File: sandbox/libv_glr_main2.cpp

// hpp
#include "libv_gl_runner.hpp"
// libv
//#include <libv/gl/gl.hpp>
#include <libv/math/angle.hpp>
#include <libv/utility/chrono_mod.hpp>
//#include <libv/utility/read_file.hpp>
// std
#include <chrono>
#include <iostream>
// pro
#include <libv/glr/attribute.hpp>
#include <libv/glr/layout_std140.hpp>
#include <libv/glr/mesh.hpp>

//#include <libv/glr/program.hpp>
#include <libv/glr/framebuffer.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/remote.hpp>
#include <libv/glr/uniform_buffer.hpp>

#include <libv/glr/procedural/sphere.hpp>
#include <libv/glr/procedural/ignore.hpp>


#include <libv/rev/post_processing.hpp>
#include <libv/rev/render_target.hpp>
#include <libv/rev/shader.hpp>
#include <libv/rev/shader_loader.hpp>

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

struct UniformsOnlyMatrixBlock {
//	libv::glr::Uniform_mat4f matMVP;
//	libv::glr::Uniform_mat4f matM;
//	libv::glr::Uniform_vec3f color;

	template <typename Access> void access_uniforms(Access& access) {
		(void) access;

//		access(matMVP, "matMVP");
//		access(render_resolution, "render_resolution");
//		access(time, "time", 0.f);
	}

	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
	}
};

struct UniformsPPTexture {
	libv::glr::Uniform_texture texture0;

	template <typename Access> void access_uniforms(Access& access) {
		access(texture0, "texture0", textureChannel_diffuse);
	}

	template <typename Access> void access_blocks(Access&) {
	}
};

// -------------------------------------------------------------------------------------------------

struct Sandbox {
	int debug_view_bloom_step = 0;
	std::chrono::duration<float> running_time{0};

	libv::vec2i window_size = {WINDOW_WIDTH, WINDOW_HEIGHT};

	libv::glr::Remote remote; // Remote has to be the first data member to clean up gl resources

	libv::glr::Mesh mesh_sphere{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	libv::glr::Mesh mesh_color_bar{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

	libv::glr::UniformBuffer uniform_stream{libv::gl::BufferUsage::StreamDraw};
	libv::rev::ShaderLoader shader_loader{"../res/shader/"};
	libv::rev::Shader<UniformsOnlyMatrixBlock> shader_sphere{shader_loader, "rev_sandbox/sphere.vs", "rev_sandbox/sphere.fs"};
	libv::rev::Shader<UniformsOnlyMatrixBlock> shader_color_bar{shader_loader, "rev_sandbox/sphere.vs", "rev_sandbox/color_bar.fs"};

	libv::rev::RenderTarget render_target{window_size, 4};

	libv::rev::PostProcessing post_processing{shader_loader, window_size};

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

//		log_sandbox.debug("{:46} [{:>10} ]", "CurrentAvailableVideoMemory",        remote.gl().getCurrentAvailableVideoMemory());
//		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnits",               remote.gl().getMaxTextureImageUnits());
//		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsFragment",       remote.gl().getMaxTextureImageUnitsFragment());
//		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsGeometry",       remote.gl().getMaxTextureImageUnitsGeometry());
//		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsVertex",         remote.gl().getMaxTextureImageUnitsVertex());
//		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsCompute",        remote.gl().getMaxTextureImageUnitsCompute());
//		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsTessControl",    remote.gl().getMaxTextureImageUnitsTessControl());
//		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsTessEvaluation", remote.gl().getMaxTextureImageUnitsTessEvaluation());
//		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureSize",                     remote.gl().getMaxTextureSize());
//		log_sandbox.debug("{:46} [{:>10} ]", "MaxUniformBlockSize",                remote.gl().getMaxUniformBlockSize());
//		log_sandbox.debug("{:46} [{:>10} ]", "MaxUniformBufferBindings",           remote.gl().getMaxUniformBufferBindings());
//		log_sandbox.debug("{:46} [{:>10} ]", "MaxVertexAttribs",                   remote.gl().getMaxVertexAttribs());
//		log_sandbox.debug("{:46} [{:>10} ]", "MaxVertexUniformComponents",         remote.gl().getMaxVertexUniformComponents());
//		log_sandbox.debug("{:46} [{:>10} ]", "UniformBufferOffsetAlignment",       remote.gl().getUniformBufferOffsetAlignment());
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
		shader_loader.update(glr.out_of_order_gl());

		const auto guard_s = glr.state.push_guard();
		const auto guard_m = glr.model.push_guard();
		const auto guard_v = glr.view.push_guard();
		const auto guard_p = glr.projection.push_guard();

		glr.state.enableBlend();
		glr.state.blendSrc_SourceAlpha();
		glr.state.blendDst_One_Minus_SourceAlpha();

		glr.state.enableCullFace();
		glr.state.frontFaceCCW();
		glr.state.cullBackFace();

		glr.state.polygonModeFill();
//		glr.state.polygonModeLine();

		glr.state.enableDepthTest();
		glr.state.depthFunctionLess();

		glr.setClearColor(0.098f, 0.2f, 0.298f, 1.0f);
//		glr.setClearColor(1.f, 1.f, 1.f, 1.0f);

		glr.projection = libv::mat4f::ortho(0, static_cast<float>(window_size.x), 0, static_cast<float>(window_size.y), -1000, +1000);
		glr.view = libv::mat4f::identity();
		glr.model = libv::mat4f::identity();

		// --- Render the Sphere ---

		glr.framebuffer_draw(render_target.framebuffer());
//		glr.framebuffer_draw_default();
		glr.viewport({0, 0}, render_target.size());
		glr.clearColor();
		glr.clearDepth();

		{
			const auto guard_m2 = glr.model.push_guard();
			glr.model.translate(static_cast<float>(window_size.x) * 0.5f, static_cast<float>(window_size.y) * 0.5f, 0);
			glr.model.scale(300.f);
			glr.model.rotate(libv::Degrees(running_time.count() * 5.f), 0, 1, 0);

			auto uniforms = uniform_stream.block_unique(layout_matrices);
			uniforms[layout_matrices.matMVP] = glr.mvp();
			uniforms[layout_matrices.matM] = glr.model;

			glr.program(shader_sphere.program());
			glr.uniform(std::move(uniforms));
			glr.render(mesh_sphere);
		}
		const auto mini_count = 16;
		const auto mini_countf = static_cast<float>(mini_count);
		for (int i = 0 ; i < mini_count; ++i) {
			const auto fi = static_cast<float>(i);
			const auto guard_m2 = glr.model.push_guard();
			glr.model.translate(static_cast<float>(window_size.x) * 0.9f + std::sin(running_time.count()) * 50.f, (0.5f + fi) / mini_countf * static_cast<float>(window_size.y), 0);
			glr.model.scale(15.f);
			glr.model.rotate(libv::Radian(libv::pi * fi / mini_countf), 0, 1, 0); // Using pi and not tau as only half rotation is enough, sphere is symmetric

			auto uniforms = uniform_stream.block_unique(layout_matrices);
			uniforms[layout_matrices.matMVP] = glr.mvp();
			uniforms[layout_matrices.matM] = glr.model;

			glr.program(shader_sphere.program());
			glr.uniform(std::move(uniforms));
			glr.render(mesh_sphere);
		}
		{
			// Color-Space Test Bar
			const auto guard_m2 = glr.model.push_guard();
			const auto window_size_f = window_size.cast<float>();
			glr.model.translate(window_size_f.x * 0.5f, window_size_f.y * 0.5f, 500);
			glr.model.scale(window_size_f.x * 0.5f, window_size_f.y * 0.5f, 0);

			glr.model.translate(0, -0.90f, 0);
			glr.model.scale(1, 0.10f, 1);

			auto uniforms = uniform_stream.block_unique(layout_matrices);
			uniforms[layout_matrices.matMVP] = glr.mvp();
			uniforms[layout_matrices.matM] = glr.model;

			glr.program(shader_color_bar.program());
			glr.uniform(std::move(uniforms));
			glr.render(mesh_color_bar);
		}

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
