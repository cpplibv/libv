// File: libv_gl_main3.cpp, Created on 2017.11.17. at 22:38, Author: Vader

// hpp
#include "libv_gl_runner.hpp"
// ext
#include <GL/glew.h>
// libv
#include <libv/gl/gl.hpp>
#include <libv/math/angle.hpp>
#include <libv/utility/read_file.hpp>
// std
#include <chrono>
#include <iostream>
// pro
#include <libv/glr/attribute.hpp>
#include <libv/glr/layout_std140.hpp>
#include <libv/glr/remote.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_block_binding.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/program.hpp>
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

in vec2 fragmentTexture0;

out vec4 output;

uniform sampler2D texture1Sampler;

void main() {
	output = texture(texture1Sampler, fragmentTexture0);
}
)";

const auto shader_font_vs = R"(
#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec4 vertexColor0;
layout(location = 8) in vec2 vertexTexture0;
layout(location = 10) in vec4 vertexFontXYWH;

out vec4 fragmentColor0;
out vec2 fragmentTexture0;
out vec4 fragmentFontXYWH;

uniform mat4 MVPmat;

void main() {
	fragmentTexture0 = vertexTexture0;
	fragmentFontXYWH = vertexFontXYWH;
	gl_Position = MVPmat * vec4(vertexPosition, 1);
}
)";

const auto shader_font_gs = R"(
#version 330 core

in vec4 fragmentColor0;
in vec2 fragmentTexture0;
in vec4 fragmentFontXYWH;

void main() {
}
)";

const auto shader_font_fs = R"(
#version 330 core

in vec4 fragmentColor0;
in vec2 fragmentTexture0;
in vec4 fragmentFontXYWH;

out vec4 output;

uniform sampler2D textureSampler;

void main() {
	output = texture(texture1Sampler, fragmentTexture0);
}
)";

constexpr auto attribute_position  = libv::glr::Attribute<0, libv::vec3f>{};
constexpr auto attribute_color0    = libv::glr::Attribute<2, libv::vec4f>{};
constexpr auto attribute_texture0  = libv::glr::Attribute<8, libv::vec2f>{};
constexpr auto attribute_font_xywh = libv::glr::Attribute<10, libv::vec4f>{};

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

	libv::glr::Program font_program;
	libv::glr::Uniform_mat4f font_uniform_MVPmat;
	libv::glr::Uniform_texture font_uniform_texture;

//	libv::glr::Font font;
//	libv::glr::Text text;

	Sandbox() {
		// Plane
		plane_program.vertex(shader_plane_vs);
		plane_program.fragment(shader_plane_fs);
		plane_program.assign(plane_uniform_MVPmat, "MVPmat");
		plane_program.assign(plane_uniform_texture, "textureSampler", textureChannel_diffuse);

		{
			auto position = plane_mesh.attribute(attribute_position);
			auto texture0 = plane_mesh.attribute(attribute_texture0);
			auto index = plane_mesh.index();

			position(0, 0, 0);
			position(WINDOW_WIDTH, 0, 0);
			position(WINDOW_WIDTH, WINDOW_HEIGHT, 0);
			position(0, WINDOW_HEIGHT, 0);

			texture0(0, 0);
			texture0(1, 0);
			texture0(1, 1);
			texture0(0, 1);

			index.quad(0, 1, 2, 3);
		}

		// Text
		font_program.vertex(shader_font_vs);
//		font_program.geometry(shader_font_gs);
		font_program.fragment(shader_font_fs);
		font_program.assign(plane_uniform_MVPmat, "MVPmat");
		font_program.assign(plane_uniform_texture, "textureSampler", textureChannel_diffuse);

		const auto dataFont = libv::read_file_or_throw("res/texture/cube_debug_transparent.dds");
//		font = libv::gl::load_font_or_throw(dataFont);
//
//		text.setFont(font);
//		text.setText("");

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
//		gl.state.blendEquation_Add();

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

		gl.projection = libv::mat4f::ortho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
		gl.view = libv::mat4f::identity();
		gl.model = libv::mat4f::identity();

		{
//			const auto guard_m2 = gl.model.push_guard();
//			gl.model.scale();
			gl.program(plane_program);
			gl.uniform(plane_uniform_MVPmat, gl.mvp());
//			gl.texture(font.texture, textureChannel_diffuse);
			gl.render(plane_mesh);
		}

		{
			gl.program(font_program);
			gl.uniform(font_uniform_MVPmat, gl.mvp());
//			gl.texture(font.texture, textureChannel_diffuse);
//			gl.render(text.mesh);
		}
	}
};

// Runner ------------------------------------------------------------------------------------------

int main() {
	std::cout << libv::logger_stream;
	return run_sandbox<Sandbox>("Sandbox libv.GL3", WINDOW_HEIGHT, WINDOW_WIDTH);
}
