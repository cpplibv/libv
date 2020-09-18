// Project: libv.glr, File: sandbox/libv_glr_main2.cpp, Author: Császár Mátyás [Vader]

// hpp
#include "libv_gl_runner.hpp"
// ext
#include <GL/glew.h>
// libv
#include <libv/gl/gl.hpp>
#include <libv/math/angle.hpp>
//#include <libv/utility/read_file.hpp>
// std
#include <chrono>
#include <iostream>
// pro
#include <libv/glr/attribute.hpp>
#include <libv/glr/layout_std140.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/procedural/progress_segmented_ring.hpp>
#include <libv/glr/program.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/remote.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_block_binding.hpp>
//#include <libv/gl/enum.hpp>
//#include <libv/gl/image.hpp>
//#include <libv/glr/attribute.hpp>
//#include <libv/glr/font.hpp>
//#include <libv/glr/procedural/cube.hpp>
//#include <libv/glr/procedural/sphere.hpp>
//#include <libv/glr/text.hpp>
//#include <libv/glr/texture.hpp>
//#include <libv/glr/uniform.hpp>
//#include <libv/glr/uniform_block_binding.hpp>
//#include <libv/glr/uniform_buffer.hpp>


// -------------------------------------------------------------------------------------------------

constexpr uint32_t WINDOW_WIDTH = 1280;
constexpr uint32_t WINDOW_HEIGHT = 800;

const auto shader_plane_vs = R"(
#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 8) in vec2 vertexTexture0;

out vec2 fragmentTexture0;

uniform mat4 MVPmat;

void main() {
	fragmentTexture0 = vertexTexture0;
	gl_Position = MVPmat * vec4(vertexPosition, 1);
}
)";

const auto shader_plane_fs = R"(
#version 330 core

#define FILL_MODE_CCW 0
#define FILL_MODE_CW 1
#define FILL_MODE_IN 2
#define FILL_MODE_OUT 3
#define FILL_MODE_FLOOR 4
#define FILL_MODE_CEIL 5

in vec2 fragmentTexture0;

out vec4 output;

uniform int fill_mode = FILL_MODE_CCW;
uniform float progress = 1.0;
uniform vec4 color_fg = vec4(1, 1, 1, 1);
uniform vec4 color_bg = vec4(0, 0, 0, 1);

void main() {
//	const float t = fragmentTexture0.x;
//	const float s = fragmentTexture0.y;

//	if (fill_mode == FILL_MODE_CCW && t < progress)
//		discard();

	output = vec4(1, fragmentTexture0, 1);
}
)";

constexpr auto attribute_position  = libv::glr::Attribute<0, libv::vec3f>{};
constexpr auto attribute_color0    = libv::glr::Attribute<2, libv::vec4f>{};
constexpr auto attribute_texture0  = libv::glr::Attribute<8, libv::vec2f>{};

const auto uniformBlock_sphere   = libv::glr::UniformBlockBinding{0, "Sphere"};

constexpr auto textureChannel_diffuse = libv::gl::TextureChannel{0};

// -------------------------------------------------------------------------------------------------

struct SphereUniformLayout {
	libv::glr::Uniform_mat4f MVPmat;
	libv::glr::Uniform_mat4f Mmat;
	libv::glr::Uniform_vec3f color;

	LIBV_REFLECTION_ACCESS(MVPmat);
	LIBV_REFLECTION_ACCESS(Mmat);
	LIBV_REFLECTION_ACCESS(color);
};
const auto plane_layout = libv::glr::layout_std140<SphereUniformLayout>(uniformBlock_sphere);

// -------------------------------------------------------------------------------------------------

struct Sandbox {
	libv::glr::Remote remote; // Remote has to be the first data member to cleanup gl resources

	libv::glr::Mesh plane_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

	libv::glr::Program plane_program;
	libv::glr::Uniform_mat4f plane_uniform_MVPmat;
	libv::glr::Uniform_texture plane_uniform_texture;

	Sandbox() {
		// Plane
		plane_program.vertex(shader_plane_vs);
		plane_program.fragment(shader_plane_fs);
		plane_program.assign(plane_uniform_MVPmat, "MVPmat");
		plane_program.assign(plane_uniform_texture, "textureSampler", textureChannel_diffuse);

		{
			plane_mesh.clear();

			auto position = plane_mesh.attribute(attribute_position);
			auto texture0 = plane_mesh.attribute(attribute_texture0);
			auto index = plane_mesh.index();

			libv::glr::ProgressSegmentedRingStyle style;

			style.segment_num = 8;
			style.resolution = 4;
			style.radius_inner = 100;
			style.radius_outer = 300;
			style.up = libv::to_rad(30.f);
//			style.CW = false;
//			style.fill_mode = libv::glr::ProgressSegmentedRingStyle::FillMode::floor;

			libv::glr::generateProgressSegmentedRing(style, position, texture0, index);
		}

		remote.create();
		remote.enableDebug();

		log_sandbox.debug("{:46} [{:>10} ]", "CurrentAvailableVideoMemory",        remote.gl().getCurrentAvailableVideoMemory());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnits",               remote.gl().getMaxTextureImageUnits());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsFragment",       remote.gl().getMaxTextureImageUnitsFragment());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsGeometry",       remote.gl().getMaxTextureImageUnitsGeometry());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsVertex",         remote.gl().getMaxTextureImageUnitsVertex());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsCompute",        remote.gl().getMaxTextureImageUnitsCompute());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsTessControl",    remote.gl().getMaxTextureImageUnitsTessControl());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureImageUnitsTessEvaluation", remote.gl().getMaxTextureImageUnitsTessEvaluation());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxTextureSize",                     remote.gl().getMaxTextureSize());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxUniformBlockSize",                remote.gl().getMaxUniformBlockSize());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxUniformBufferBindings",           remote.gl().getMaxUniformBufferBindings());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxVertexAttribs",                   remote.gl().getMaxVertexAttribs());
		log_sandbox.debug("{:46} [{:>10} ]", "MaxVertexUniformComponents",         remote.gl().getMaxVertexUniformComponents());
		log_sandbox.debug("{:46} [{:>10} ]", "UniformBufferOffsetAlignment",       remote.gl().getUniformBufferOffsetAlignment());
	}

	~Sandbox() {
	}

	void update(const std::chrono::duration<float> deltaTime) {
		(void) deltaTime;
	}

	void render() {
		auto queue = remote.queue();

		render_remote(queue);

		remote.queue(std::move(queue));
		remote.execute();
	}

	void render_remote(libv::glr::Queue& gl) {
		const auto guard_s = gl.state.push_guard();
		const auto guard_m = gl.model.push_guard();
		const auto guard_v = gl.view.push_guard();
		const auto guard_p = gl.projection.push_guard();

		gl.state.enableBlend();
		gl.state.blendSrc_SourceAlpha();
		gl.state.blendDst_One_Minus_SourceAlpha();

		gl.state.enableCullFace();
		gl.state.frontFaceCCW();
		gl.state.cullBackFace();

		gl.state.polygonModeFill();
//		gl.state.polygonModeLine();

		gl.state.enableDepthTest();
		gl.state.depthFunctionLess();

		gl.setClearColor(0.098f, 0.2f, 0.298f, 1.0f);
		gl.clearColor();
		gl.clearDepth();

		gl.viewport({0, 0}, {WINDOW_WIDTH, WINDOW_HEIGHT});

		gl.projection = libv::mat4f::ortho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
		gl.view = libv::mat4f::identity();
		gl.model = libv::mat4f::identity();

		{
			const auto guard_m = gl.model.push_guard();
			gl.model.translate(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f, 0);

			gl.program(plane_program);
			gl.uniform(plane_uniform_MVPmat, gl.mvp());
			gl.render(plane_mesh);
		}
	}
};

// Runner ------------------------------------------------------------------------------------------

int main() {
	std::cout << libv::logger_stream;
	return run_sandbox<Sandbox>("Sandbox libv.GLR2", WINDOW_HEIGHT, WINDOW_WIDTH);
}
