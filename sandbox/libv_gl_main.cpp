// Project: libv.gl, File: sandbox/libv_gl_main.cpp

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
#include <libv/gl/check.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/program.hpp>
#include <libv/gl/shader.hpp>
#include <libv/gl/texture.hpp>
#include <libv/gl/uniform.hpp>
#include <libv/gl/vertex_array.hpp>


// -------------------------------------------------------------------------------------------------

constexpr uint32_t WINDOW_WIDTH = 1280;
constexpr uint32_t WINDOW_HEIGHT = 800;

// -------------------------------------------------------------------------------------------------

struct Sandbox {
	float angle = 0.f;

	libv::gl::GL gl;

	libv::gl::Attribute<0, libv::vec3f> attributePosition;
	libv::gl::Attribute<2, libv::vec4f> attributeColor;
	libv::gl::Attribute<8, libv::vec2f> attributeTex0;

	libv::gl::Shader shaderTest0Frag;
	libv::gl::Shader shaderTest0Vert;
	libv::gl::Program programTest0;
	libv::gl::Uniform_mat4f uniformTest0matMVP;

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

	libv::gl::Shader shaderDepthFrag;
	libv::gl::Shader shaderDepthVert;
	libv::gl::Program programDepth;
	libv::gl::Uniform_mat4f uniformDepthmatMVP;

	libv::gl::ArrayBuffer bufferVertexData;
	libv::gl::ArrayBuffer bufferVertexIndices;
	libv::gl::VertexArray vertexArray;

	libv::gl::Texture2D texture0;
	libv::gl::TextureCube textureSky;

	struct Vertex {
		libv::vec3f position;
		libv::vec4f color;
		libv::vec2f uv;
	};

	Sandbox() {
		gl.capability.blend.enable();
		gl.capability.cull.enable();
		gl.capability.depthTest.enable();
		gl.depthFunction.less();
		gl.blendFunction(libv::gl::BlendFunction::SourceAlpha, libv::gl::BlendFunction::One_Minus_SourceAlpha);
		gl.cullFace.back();
		gl.frontFace.ccw();
		gl.polygonMode(true ? libv::gl::PolygonMode::Fill : libv::gl::PolygonMode::Line);

		Vertex dataVertex[]{
			Vertex{libv::vec3f(-1.f, -1.f, 0.f), libv::vec4f(1.f, 0.f, 0.f, 1.f), libv::vec2f(0.f, 0.f)},
			Vertex{libv::vec3f(+1.f, -1.f, 0.f), libv::vec4f(1.f, 1.f, 0.f, 1.f), libv::vec2f(1.f, 0.f)},
			Vertex{libv::vec3f(+1.f, +1.f, 0.f), libv::vec4f(0.f, 1.f, 1.f, 1.f), libv::vec2f(1.f, 1.f)},
			Vertex{libv::vec3f(-1.f, +1.f, 0.f), libv::vec4f(0.f, 0.f, 1.f, 1.f), libv::vec2f(0.f, 1.f)}
		};

		uint32_t dataIndices[]{0, 1, 2, 0, 2, 3};

		uint8_t dataTexture[]{
			255, 0, 0, 255,
			0, 255, 0, 255,
			0, 0, 255, 255,
			0, 0, 0, 255
		};

		uint32_t dataTextureSkyX1[]{255, 4286545919, 4286545919, 4286545919};
		uint32_t dataTextureSkyX0[]{255, 8355839, 8355839, 8355839};
		uint32_t dataTextureSkyY1[]{255, 2147450879, 2147450879, 2147450879};
		uint32_t dataTextureSkyY0[]{255, 2130739199, 2130739199, 2130739199};
		uint32_t dataTextureSkyZ1[]{255, 2139095039, 2139095039, 2139095039};
		uint32_t dataTextureSkyZ0[]{255, 2139029759, 2139029759, 2139029759};

		gl(shaderTest0Frag).create(libv::gl::ShaderType::Fragment);
		gl(shaderTest0Frag).compile(libv::read_file_or_throw("res/shader/sandbox_gl/test0.fs"));
		gl(shaderTest0Vert).create(libv::gl::ShaderType::Vertex);
		gl(shaderTest0Vert).compile(libv::read_file_or_throw("res/shader/sandbox_gl/test0.vs"));
		gl(programTest0).create();
		gl(programTest0).link(shaderTest0Vert, shaderTest0Frag);
		gl(programTest0).assign(uniformTest0matMVP, "matMVP");

		gl(shaderTest1Frag).create(libv::gl::ShaderType::Fragment);
		gl(shaderTest1Frag).compile(libv::read_file_or_throw("res/shader/sandbox_gl/test1.fs"));
		gl(shaderTest1Vert).create(libv::gl::ShaderType::Vertex);
		gl(shaderTest1Vert).compile(libv::read_file_or_throw("res/shader/sandbox_gl/test1.vs"));
		gl(programTest1).create();
		gl(programTest1).link(shaderTest1Frag, shaderTest1Vert);
		gl(programTest1).assign(uniformTest1matMVP, "matMVP");
		gl(programTest1).assign(uniformTest1TextureDiffuseSampler, "textureDiffuseSampler");

		gl(shaderTest2Frag).create(libv::gl::ShaderType::Fragment);
		gl(shaderTest2Frag).compile(libv::read_file_or_throw("res/shader/sandbox_gl/test2.fs"));
		gl(shaderTest2Vert).create(libv::gl::ShaderType::Vertex);
		gl(shaderTest2Vert).compile(libv::read_file_or_throw("res/shader/sandbox_gl/test2.vs"));
		gl(programTest2).create();
		gl(programTest2).link(shaderTest2Frag, shaderTest2Vert);
		gl(programTest2).assign(uniformTest2matMVP, "matMVP");
		gl(programTest2).assign(uniformTest2matM, "matM");
		gl(programTest2).assign(uniformTest2EyePosW, "eyePosW");
		gl(programTest2).assign(uniformTest2TextureSkySampler, "textureSkySampler");

		gl(shaderDepthFrag).create(libv::gl::ShaderType::Fragment);
		gl(shaderDepthFrag).compile(libv::read_file_or_throw("res/shader/sandbox_gl/depth.fs"));
		gl(shaderDepthVert).create(libv::gl::ShaderType::Vertex);
		gl(shaderDepthVert).compile(libv::read_file_or_throw("res/shader/sandbox_gl/depth.vs"));
		gl(programDepth).create();
		gl(programDepth).link(shaderDepthFrag, shaderDepthVert);
		gl(programDepth).assign(uniformDepthmatMVP, "matMVP");

		gl(bufferVertexData).create();
		gl(bufferVertexData).bind();
//		gl(bufferVertexData).data(&dataVertex[0], sizeof(dataVertex[0]) * std::size(dataVertex), libv::gl::BufferUsage::StaticDraw);
		const auto vertexDataSize = sizeof(dataVertex[0]) * std::size(dataVertex);
		gl(bufferVertexData).data(nullptr, vertexDataSize, libv::gl::BufferUsage::StaticDraw);
		auto* vramVertexData = gl(bufferVertexData).dsa_map(libv::gl::BufferAccessFull::Write);
//		auto* vramVertexData = gl(bufferVertexData).mapRange(
//				0,
//				vertexDataSize,
//				libv::gl::BufferAccess::Write |
//				libv::gl::BufferAccess::InvalidateBuffer |
//				libv::gl::BufferAccess::Unsynchronized);
		if (!vramVertexData)
			throw std::runtime_error("Failed to map buffer data");
		std::memcpy(vramVertexData, dataVertex, vertexDataSize);
		auto vramSuccess = gl(bufferVertexData).dsa_unmap();
		if (!vramSuccess)
			throw std::runtime_error("Failed to unmap buffer data");

		gl(bufferVertexIndices).create();
		gl(bufferVertexIndices).bind();
//		gl(bufferVertexIndices).data(&dataIndices[0], sizeof(dataIndices[0]) * std::size(dataIndices), libv::gl::BufferUsage::StaticDraw);
		gl(bufferVertexIndices).data(dataIndices, libv::gl::BufferUsage::StaticDraw);

		gl(vertexArray).create();
		gl(vertexArray).bind();
		gl(vertexArray).bindAttribute(bufferVertexData, attributePosition, sizeof(Vertex), offsetof(Vertex, position));
		gl(vertexArray).bindAttribute(bufferVertexData, attributeColor, sizeof(Vertex), offsetof(Vertex, color));
		gl(vertexArray).bindAttribute(bufferVertexData, attributeTex0, sizeof(Vertex), offsetof(Vertex, uv));
		gl(vertexArray).bindElements(bufferVertexIndices);

		gl(texture0).create();
		gl(texture0).bind();
		gl(texture0).storage(1, libv::gl::FormatSized::RGBA8, 2, 2);
		gl(texture0).subImage(0, 0, 0, 2, 2, libv::gl::FormatBase::RGBA, libv::gl::DataType::U8, dataTexture);
		gl(texture0).setMinFilter(libv::gl::MinFilter::Linear);
		gl(texture0).setMagFilter(libv::gl::MagFilter::Linear);
		gl(texture0).setWrapS(libv::gl::Wrap::ClampToEdge);
		gl(texture0).setWrapT(libv::gl::Wrap::ClampToEdge);

		gl(textureSky).create();
		gl(textureSky).bind();
		gl(textureSky).storage(1, libv::gl::FormatCompressed::RGBA_S3TC_DXT5_EXT, 2, 2);
		gl(textureSky).subImage(libv::gl::CubeSide::PositiveX, 0, 0, 0, 2, 2, libv::gl::FormatBase::RGBA, libv::gl::DataType::U32_R8_G8_B8_A8, dataTextureSkyX1);
		gl(textureSky).subImage(libv::gl::CubeSide::NegativeX, 0, 0, 0, 2, 2, libv::gl::FormatBase::RGBA, libv::gl::DataType::U32_R8_G8_B8_A8, dataTextureSkyX0);
		gl(textureSky).subImage(libv::gl::CubeSide::PositiveY, 0, 0, 0, 2, 2, libv::gl::FormatBase::RGBA, libv::gl::DataType::U32_R8_G8_B8_A8, dataTextureSkyY1);
		gl(textureSky).subImage(libv::gl::CubeSide::NegativeY, 0, 0, 0, 2, 2, libv::gl::FormatBase::RGBA, libv::gl::DataType::U32_R8_G8_B8_A8, dataTextureSkyY0);
		gl(textureSky).subImage(libv::gl::CubeSide::PositiveZ, 0, 0, 0, 2, 2, libv::gl::FormatBase::RGBA, libv::gl::DataType::U32_R8_G8_B8_A8, dataTextureSkyZ1);
		gl(textureSky).subImage(libv::gl::CubeSide::NegativeZ, 0, 0, 0, 2, 2, libv::gl::FormatBase::RGBA, libv::gl::DataType::U32_R8_G8_B8_A8, dataTextureSkyZ0);
	}

	~Sandbox() {
		gl(programTest0).destroy();
		gl(programTest1).destroy();
		gl(programTest2).destroy();
		gl(programDepth).destroy();

		gl(shaderTest0Frag).destroy();
		gl(shaderTest0Vert).destroy();
		gl(shaderTest1Frag).destroy();
		gl(shaderTest1Vert).destroy();
		gl(shaderTest2Frag).destroy();
		gl(shaderTest2Vert).destroy();
		gl(shaderDepthFrag).destroy();
		gl(shaderDepthVert).destroy();

		gl(texture0).destroy();
		gl(textureSky).destroy();

		gl(vertexArray).destroy();
		gl(bufferVertexData).destroy();
		gl(bufferVertexIndices).destroy();
	}

	void update(const std::chrono::duration<float> deltaTime) {
		angle += 22.5f * deltaTime.count();
	}

	void render() {
		gl.clearColor(0.098f, 0.2f, 0.298f, 1.0f);
		gl.clear();

		auto pStackGuard = gl.projection.push_guard();
		auto vStackGuard = gl.view.push_guard();
		auto mStackGuard = gl.model.push_guard();

		gl.projection = libv::mat4f::perspective(1.f, 1.f * WINDOW_WIDTH / WINDOW_HEIGHT, 1.f, 1000.f);
		gl.view = libv::mat4f::lookAt({5.f, 3.f, 5.f}, {0.f, 0.f, 0.f}, {0.f, 1.f, 0.f});
		gl.view.rotate(libv::degrees{angle}, 0.f, 1.f, 0.f);
		gl.model = libv::mat4f::identity();
		// -----------------------------------------------------------------------------------------

		// TODO P4: Binding a texture to an uniform instead of TextureType sounds like a good idea.

		gl.model.translate(libv::vec3f(-1, -1, +0));

		{
			gl(programTest0).use();
			uniformTest0matMVP = gl.mvp();
			gl(vertexArray).bind();
			gl(vertexArray).drawElements(libv::gl::Primitive::Triangles, 0, 6);
			gl(vertexArray).unbind();
		}

		gl.model.translate(libv::vec3f(+0, +2, +0));

		{
			gl(programTest1).use();
			gl.activeTexture(libv::gl::TextureUnit::diffuse);
			gl(texture0).bind();
			uniformTest1matMVP = gl.mvp();
			uniformTest1TextureDiffuseSampler = libv::gl::TextureUnit::diffuse;

			gl(vertexArray).bind();
			gl(vertexArray).drawElements(libv::gl::Primitive::Triangles, 0, 6);
			gl(vertexArray).unbind();
		}

		gl.model.translate(libv::vec3f(+2, +0, +0));

		{
			gl(programTest2).use();
			gl.activeTexture(libv::gl::TextureUnit::environment);
			gl(textureSky).bind();
			uniformTest2matMVP = gl.mvp();
			uniformTest2matM = gl.model;
			uniformTest2EyePosW = gl.eye();
			uniformTest2TextureSkySampler = libv::gl::TextureUnit::environment;

			gl(vertexArray).bind();
			gl(vertexArray).drawElements(libv::gl::Primitive::Triangles, 0, 6);
			gl(vertexArray).unbind();
		}

		libv::gl::checkGL();
	}
};

// Runner ------------------------------------------------------------------------------------------

int main() {
	std::cout << libv::logger_stream;
	return run_sandbox<Sandbox>("Sandbox libv.GL1", WINDOW_WIDTH, WINDOW_HEIGHT);
}
