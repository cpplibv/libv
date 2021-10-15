// Project: libv.gl, File: sandbox/libv_gl_main2.cpp

// hpp
#include "libv_gl_runner.hpp"
// libv
#include <libv/math/angle.hpp>
#include <libv/utility/read_file.hpp>
// std
#include <chrono>
#include <iostream>
// pro
#include <libv/gl/array_buffer.hpp>
#include <libv/gl/attribute.hpp>
#include <libv/gl/check.hpp>
#include <libv/gl/framebuffer.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/image.hpp>
#include <libv/gl/load_image.hpp>
#include <libv/gl/program.hpp>
#include <libv/gl/renderbuffer.hpp>
#include <libv/gl/shader.hpp>
#include <libv/gl/texture.hpp>
#include <libv/gl/uniform.hpp>
#include <libv/gl/vertex_array.hpp>


// -------------------------------------------------------------------------------------------------

constexpr uint32_t WINDOW_WIDTH = 1280;
constexpr uint32_t WINDOW_HEIGHT = 800;

// -------------------------------------------------------------------------------------------------

constexpr auto shader_test1_vs = R"(
#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 8) in vec2 vertexUV;

uniform mat4 matMVP;

out vec2 fragmentUV;

void main() {
	gl_Position = matMVP * vec4(vertexPos, 1);
	fragmentUV = vertexUV;
})";

constexpr auto shader_test1_fs = R"(
#version 330 core

uniform sampler2D textureDiffuseSampler;

in vec2 fragmentUV;

out vec4 color;

void main() {
	color = vec4(1, 1, 1, 1) * 0.75 + texture(textureDiffuseSampler, fragmentUV, 0).rgba * 0.25;
})";

// =================================================================================================

constexpr auto shader_quad_vs = R"(
#version 330 core

layout(location = 0) in vec3 vertexPos;

out vec2 fragmentUV;

void main() {
	gl_Position = vec4(vertexPos, 1);
	fragmentUV = vertexPos.xy * 0.5 + 0.5;
})";

constexpr auto shader_quad_fs = R"(
#version 330 core

uniform sampler2D textureSampler;

in vec2 fragmentUV;

out vec4 color;

void main() {
	color = texture(textureSampler, fragmentUV, 0).rgba;
})";

// -------------------------------------------------------------------------------------------------

struct Sandbox {
	float angle = 0.f;
	size_t frame_counter = 0;

	libv::gl::GL gl;

	libv::gl::AttributeFixLocation<libv::vec3f> attributePosition;
	libv::gl::AttributeFixLocation<libv::vec2f> attributeUV;

	int32_t sample_count = 4;
	libv::vec2i sizeFBOMS = libv::vec2i{WINDOW_WIDTH, WINDOW_HEIGHT} / 8;
//	libv::vec2i sizeFBOMS = libv::vec2i{WINDOW_WIDTH, WINDOW_HEIGHT} / 4;
//	libv::vec2i sizeFBOMS = {WINDOW_WIDTH, WINDOW_HEIGHT};
//	libv::vec2i sizeFBOMS = {256, 256};

	libv::gl::Framebuffer framebuffer;
//	libv::gl::TextureRectangle textureFBOColor;
//	libv::gl::TextureRectangle renderFBODepth;
	libv::gl::Texture2D textureFBOColor;
	libv::gl::Renderbuffer renderFBODepth;

	libv::gl::Framebuffer framebufferMS;
	libv::gl::Texture2DMultisample textureFBOMSColor;
	libv::gl::Renderbuffer renderFBOMSDepth;

	libv::gl::Shader shaderQuadFrag;
	libv::gl::Shader shaderQuadVert;
	libv::gl::Program programQuad;
	libv::gl::Uniform_texture uniformQuadTextureDiffuseSampler;

	libv::gl::Shader shaderTest1Frag;
	libv::gl::Shader shaderTest1Vert;
	libv::gl::Program programTest1;
	libv::gl::Uniform_mat4f uniformTest1matMVP;
	libv::gl::Uniform_texture uniformTest1TextureDiffuseSampler;

	libv::gl::Shader shaderTest2Frag;
	libv::gl::Shader shaderTest2Vert;
	libv::gl::Program programTest2;
	libv::gl::Uniform_mat4f uniformTest2matMVP;
	libv::gl::Uniform_mat4f uniformTest2matM;
	libv::gl::Uniform_vec3f uniformTest2EyePosW;
	libv::gl::Uniform_texture uniformTest2TextureSkySampler;

	libv::gl::ArrayBuffer bufferVertexData;
	libv::gl::ArrayBuffer bufferVertexIndices;
	libv::gl::VertexArray vertexArray;

	libv::gl::ArrayBuffer bufferVertexQuadData;
	libv::gl::ArrayBuffer bufferVertexQuadIndices;
	libv::gl::VertexArray vertexArrayQuad;

	libv::gl::Texture2D texturePlane;
	libv::gl::TextureCube textureSky;

	struct Vertex {
		libv::vec3f position;
		libv::vec2f uv;

		Vertex(libv::vec3f position, libv::vec2f uv) :
			position(position), uv(uv) { }
	};

	Sandbox() {
		gl.enableDebug();

		gl.capability.blend.enable();
		gl.capability.cullFace.enable();
		gl.capability.depthTest.enable();
		gl.depthFunction.less();
		gl.blendFunction(libv::gl::BlendFunction::SourceAlpha, libv::gl::BlendFunction::One_Minus_SourceAlpha);

//		gl.blend.source(libv::gl::BlendFunction::SourceAlpha);
//		gl.blend.destination(libv::gl::BlendFunction::One_Minus_SourceAlpha);
//
//		gl.blend.source.sourceAlpha();
//		gl.blend.destination.one_Minus_SourceAlpha();

		gl.cullFace.back();
		gl.frontFace.ccw();
		gl.polygonMode(true ? libv::gl::PolygonMode::Fill : libv::gl::PolygonMode::Line);

		Vertex dataVertex[]{
			Vertex{libv::vec3f(-1.f, -1.f, +1.f), libv::vec2f(0.f, 0.f)},
			Vertex{libv::vec3f(+1.f, -1.f, +1.f), libv::vec2f(1.f, 0.f)},
			Vertex{libv::vec3f(+1.f, +1.f, +1.f), libv::vec2f(1.f, 1.f)},
			Vertex{libv::vec3f(-1.f, +1.f, +1.f), libv::vec2f(0.f, 1.f)},

			Vertex{libv::vec3f(+1.f, +1.f, -1.f), libv::vec2f(0.f, 0.f)},
			Vertex{libv::vec3f(+1.f, -1.f, -1.f), libv::vec2f(1.f, 0.f)},
			Vertex{libv::vec3f(-1.f, -1.f, -1.f), libv::vec2f(1.f, 1.f)},
			Vertex{libv::vec3f(-1.f, +1.f, -1.f), libv::vec2f(0.f, 1.f)},

			Vertex{libv::vec3f(+1.f, +1.f, +1.f), libv::vec2f(1.f, 1.f)},
			Vertex{libv::vec3f(+1.f, +1.f, -1.f), libv::vec2f(1.f, 0.f)},
			Vertex{libv::vec3f(-1.f, +1.f, -1.f), libv::vec2f(0.f, 0.f)},
			Vertex{libv::vec3f(-1.f, +1.f, +1.f), libv::vec2f(0.f, 1.f)},

			Vertex{libv::vec3f(-1.f, -1.f, -1.f), libv::vec2f(1.f, 1.f)},
			Vertex{libv::vec3f(+1.f, -1.f, -1.f), libv::vec2f(1.f, 0.f)},
			Vertex{libv::vec3f(+1.f, -1.f, +1.f), libv::vec2f(0.f, 0.f)},
			Vertex{libv::vec3f(-1.f, -1.f, +1.f), libv::vec2f(0.f, 1.f)},

			Vertex{libv::vec3f(+1.f, -1.f, -1.f), libv::vec2f(0.f, 0.f)},
			Vertex{libv::vec3f(+1.f, +1.f, -1.f), libv::vec2f(1.f, 0.f)},
			Vertex{libv::vec3f(+1.f, +1.f, +1.f), libv::vec2f(1.f, 1.f)},
			Vertex{libv::vec3f(+1.f, -1.f, +1.f), libv::vec2f(0.f, 1.f)},

			Vertex{libv::vec3f(-1.f, +1.f, +1.f), libv::vec2f(0.f, 0.f)},
			Vertex{libv::vec3f(-1.f, +1.f, -1.f), libv::vec2f(1.f, 0.f)},
			Vertex{libv::vec3f(-1.f, -1.f, -1.f), libv::vec2f(1.f, 1.f)},
			Vertex{libv::vec3f(-1.f, -1.f, +1.f), libv::vec2f(0.f, 1.f)}
		};

		uint32_t dataIndices[]{
			 0,  1,  2,  0,  2,  3,
			 4,  5,  6,  4,  6,  7,
			 8,  9, 10,  8, 10, 11,
			12, 13, 14, 12, 14, 15,
			16, 17, 18, 16, 18, 19,
			20, 21, 22, 20, 22, 23
		};

		libv::vec3f dataVertexQuad[]{
			libv::vec3f(-1, -1, 0),
			libv::vec3f(+1, -1, 0),
			libv::vec3f(+1, +1, 0),
			libv::vec3f(-1, +1, 0)
		};

		uint32_t dataIndicesQuad[]{
			 0,  1,  2,  0,  2,  3
		};

		attributePosition = 0;
		attributeUV = 8;

		{
			gl(textureFBOColor).create();
			gl(textureFBOColor).bind();
			gl(textureFBOColor).storage(1, libv::gl::FormatSized::RGB8, sizeFBOMS.x, sizeFBOMS.y);
			gl(textureFBOColor).setMagFilter(libv::gl::MagFilter::Nearest);
			gl(textureFBOColor).setMinFilter(libv::gl::MinFilter::Nearest);
			gl(textureFBOColor).setWrap(libv::gl::Wrap::ClampToEdge, libv::gl::Wrap::ClampToEdge);

			gl(renderFBODepth).create();
			gl(renderFBODepth).bind();
			gl(renderFBODepth).storage(libv::gl::FormatDepth::DEPTH_COMPONENT32, sizeFBOMS);

			gl(framebuffer).create();
			gl(framebuffer).bind();
			gl(framebuffer).attach2D(libv::gl::Attachment::Color0, textureFBOColor);
			gl(framebuffer).attach(libv::gl::Attachment::Depth, renderFBODepth);
		}

		{
			gl(textureFBOMSColor).create();
			gl(textureFBOMSColor).bind();
			gl(textureFBOMSColor).storage_ms(libv::gl::FormatSized::RGB8, sizeFBOMS, sample_count, true);

			gl(renderFBOMSDepth).create();
			gl(renderFBOMSDepth).bind();
			gl(renderFBOMSDepth).storage_ms(libv::gl::FormatDepth::DEPTH_COMPONENT32, sizeFBOMS, sample_count);

			gl(framebufferMS).create();
			gl(framebufferMS).bind();
			gl(framebufferMS).attach2D(libv::gl::Attachment::Color0, textureFBOMSColor);
			gl(framebufferMS).attach(libv::gl::Attachment::Depth, renderFBOMSDepth);
		}

		gl(shaderQuadVert).create(libv::gl::ShaderType::Vertex);
		gl(shaderQuadVert).compile(shader_quad_vs);
		gl(shaderQuadFrag).create(libv::gl::ShaderType::Fragment);
		gl(shaderQuadFrag).compile(shader_quad_fs);
		gl(programQuad).create();
		gl(programQuad).link(shaderQuadVert, shaderQuadFrag);
		gl(programQuad).assign(uniformQuadTextureDiffuseSampler, "textureSampler");

		gl(shaderTest1Vert).create(libv::gl::ShaderType::Vertex);
		gl(shaderTest1Vert).compile(shader_test1_vs);
		gl(shaderTest1Frag).create(libv::gl::ShaderType::Fragment);
		gl(shaderTest1Frag).compile(shader_test1_fs);
		gl(programTest1).create();
		gl(programTest1).link(shaderTest1Vert, shaderTest1Frag);
		gl(programTest1).assign(uniformTest1matMVP, "matMVP");
		gl(programTest1).assign(uniformTest1TextureDiffuseSampler, "textureDiffuseSampler");

		gl(shaderTest2Frag).create(libv::gl::ShaderType::Fragment);
		gl(shaderTest2Frag).compile(libv::read_file_or_throw("res/shader/test2.fs"));
		gl(shaderTest2Vert).create(libv::gl::ShaderType::Vertex);
		gl(shaderTest2Vert).compile(libv::read_file_or_throw("res/shader/test2.vs"));
		gl(programTest2).create();
		gl(programTest2).link(shaderTest2Frag, shaderTest2Vert);
		gl(programTest2).assign(uniformTest2matMVP, "matMVP");
		gl(programTest2).assign(uniformTest2matM, "matM");
		gl(programTest2).assign(uniformTest2EyePosW, "eyePosW");
		gl(programTest2).assign(uniformTest2TextureSkySampler, "textureSkySampler");

		gl(bufferVertexData).create();
		gl(bufferVertexData).bind();
		gl(bufferVertexData).data(&dataVertex[0], sizeof(dataVertex), libv::gl::BufferUsage::StaticDraw);
		gl(bufferVertexIndices).create();
		gl(bufferVertexIndices).bind();
		gl(bufferVertexIndices).data(&dataIndices[0], sizeof(dataIndices), libv::gl::BufferUsage::StaticDraw);

		gl(vertexArray).create();
		gl(vertexArray).bind();
		gl(vertexArray).bindAttribute(bufferVertexData, attributePosition, sizeof(Vertex), offsetof(Vertex, position));
		gl(vertexArray).bindAttribute(bufferVertexData, attributeUV, sizeof(Vertex), offsetof(Vertex, uv));
		gl(vertexArray).bindElements(bufferVertexIndices);

		gl(bufferVertexQuadData).create();
		gl(bufferVertexQuadData).bind();
		gl(bufferVertexQuadData).data(&dataVertexQuad[0], sizeof(dataVertexQuad), libv::gl::BufferUsage::StaticDraw);
		gl(bufferVertexQuadIndices).create();
		gl(bufferVertexQuadIndices).bind();
		gl(bufferVertexQuadIndices).data(&dataIndicesQuad[0], sizeof(dataIndicesQuad), libv::gl::BufferUsage::StaticDraw);

		gl(vertexArrayQuad).create();
		gl(vertexArrayQuad).bind();
		gl(vertexArrayQuad).bindAttribute(bufferVertexQuadData, attributePosition, sizeof(libv::vec3f), 0);
		gl(vertexArrayQuad).bindElements(bufferVertexQuadIndices);

		auto dataPlane = libv::read_file_or_throw("res/texture/hexagon_metal_0001_diffuse.dds");
		auto imagePlane = libv::gl::load_image_or_throw(dataPlane);
		texturePlane = libv::gl::Texture2D(imagePlane.createTexture());

		auto dataSky = libv::read_file_or_throw("res/texture/cube_nebula_green_0001.dds");
		auto imageSky = libv::gl::load_image_or_throw(dataSky);
		textureSky = libv::gl::TextureCube(imageSky.createTexture());

		gl(programQuad).use();
		uniformQuadTextureDiffuseSampler = libv::gl::TextureChannel::diffuse;

		gl(programTest1).use();
		uniformTest1TextureDiffuseSampler = libv::gl::TextureChannel::diffuse;

		gl(programTest2).use();
		uniformTest2TextureSkySampler = libv::gl::TextureChannel::sky;

		libv::gl::checkGL();
	}

	~Sandbox() {
		gl(programQuad).destroy();
		gl(programTest1).destroy();
		gl(programTest2).destroy();

		gl(shaderQuadFrag).destroy();
		gl(shaderQuadVert).destroy();
		gl(shaderTest1Frag).destroy();
		gl(shaderTest1Vert).destroy();
		gl(shaderTest2Frag).destroy();
		gl(shaderTest2Vert).destroy();

		gl(texturePlane).destroy();
		gl(textureSky).destroy();

		gl(framebufferMS).destroy();
		gl(textureFBOMSColor).destroy();
		gl(renderFBOMSDepth).destroy();

		gl(framebuffer).destroy();
		gl(textureFBOColor).destroy();
		gl(renderFBODepth).destroy();

		gl(vertexArray).destroy();
		gl(bufferVertexData).destroy();
		gl(bufferVertexIndices).destroy();

		gl(vertexArrayQuad).destroy();
		gl(bufferVertexQuadData).destroy();
		gl(bufferVertexQuadIndices).destroy();
	}

	void update(const std::chrono::duration<float> deltaTime) {
		frame_counter++;
		angle += 22.5f * deltaTime.count();
	}

	void render() {
		gl.clearColor(0.098f, 0.2f, 0.298f, 1.0f);
		gl.capability.multisample(frame_counter % 240 < 120);

		{
			gl(framebufferMS).bind();
			gl.viewport({0, 0}, sizeFBOMS);
			gl.clear();

			render_pass1();
			gl(framebufferMS).unbind();

			libv::gl::checkGL();
		}

		{
			gl(framebufferMS).bind();
			gl.blit({100, 100}, {100, 100}, {200, 100}, {100, 100}, libv::gl::BufferBit::Color, libv::gl::MagFilter::Nearest);
			gl(framebufferMS).unbind();

			libv::gl::checkGL();
		}

		{
			gl(framebufferMS).bind_read();
			gl(framebuffer).bind_draw();
			gl.clear();
			gl.blit({}, sizeFBOMS, {}, sizeFBOMS, libv::gl::BufferBit::Color, libv::gl::MagFilter::Nearest);

			gl(framebuffer).unbind_draw();
			gl(framebufferMS).unbind_read();

			libv::gl::checkGL();
		}

		{
			gl.framebuffer_default();
			gl.viewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
			gl.clear();

			render_pass2();

			libv::gl::checkGL();
		}

//		{
//			gl.bindFramebufferDefaultDraw();
//			gl(framebufferMS).bind_read();
//			gl.blit({}, sizeFBOMS, {}, {WINDOW_WIDTH, WINDOW_HEIGHT}, libv::gl::BufferBit::Color, libv::gl::MagFilter::Nearest);
//
//			libv::gl::checkGL();
//		}

		if (frame_counter % 120 == 0) {
			libv::vec4uc pixel;
			gl.readPixels(libv::vec2i{WINDOW_WIDTH, WINDOW_HEIGHT} / 2, {1, 1}, libv::gl::ReadFormat::RGBA, libv::gl::DataType::U32_A8_B8_G8_R8, &pixel);
			log_sandbox.trace("Center pixel color: {}", libv::vec4i{pixel});
		}
	}

	void render_pass1() {
		const auto pStackGuard = gl.projection.push_guard();
		const auto vStackGuard = gl.view.push_guard();
		const auto mStackGuard = gl.model.push_guard();

		gl.projection = libv::mat4f::perspective(1.f, 1.f * WINDOW_WIDTH / WINDOW_HEIGHT, 1.f, 1000.f);
		gl.view = libv::mat4f::lookAt({2.f, 1.75f, 2.f}, {0.f, 0.f, 0.f}, {0.f, 1.f, 0.f});
		gl.view.rotate(libv::Degrees{angle}, 0.f, 1.f, 0.f);
		gl.model = libv::mat4f::identity();

		// Draw Sky
		{
			auto eye = gl.eye();

			auto gurad_modelStack = gl.model.push_guard();
			gl.model.translate(eye);
			gl.model.scale(libv::vec3f(3.f, 3.f, 3.f));

			auto guard_depth = gl.capability.depthTest.disable_guard();
			auto guard_frontFace = gl.frontFace.cw_guard(); // Cheat for the sake of sandbox
			gl(programTest2).use();
			gl.activeTexture(libv::gl::TextureChannel::sky);
			gl(textureSky).bind();
			uniformTest2matMVP = gl.mvp();
			uniformTest2matM = gl.model;
			uniformTest2EyePosW = eye;

			gl(vertexArray).bind();
			gl(vertexArray).drawElements(libv::gl::Primitive::Triangles, 36, 0);
		}

		// Draw world
		{
			gl(programTest1).use();
			gl.activeTexture(libv::gl::TextureChannel::diffuse);
			gl(texturePlane).bind();
			uniformTest1matMVP = gl.mvp();

			gl(vertexArray).bind();
			gl(vertexArray).drawElements(libv::gl::Primitive::Triangles, 36, 0);
		}
		libv::gl::checkGL();
	}

	void render_pass2() {
		// Draw quad
		{
			gl(programQuad).use();
			gl.activeTexture(libv::gl::TextureChannel::diffuse);
//			gl(textureFBOMSColor).bind();
			gl(textureFBOColor).bind();

			gl(vertexArrayQuad).bind();
			gl(vertexArrayQuad).drawElements(libv::gl::Primitive::Triangles, 6, 0);
		}
		libv::gl::checkGL();
	}
};

// Runner ------------------------------------------------------------------------------------------

int main() {
	std::cout << libv::logger_stream;
	return run_sandbox<Sandbox>("Sandbox libv.GL2", WINDOW_HEIGHT, WINDOW_WIDTH);
}
