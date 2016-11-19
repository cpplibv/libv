// File: Main.cpp, Created on 2014.04.25. at 21:23, Author: Vader

// ext
#include <boost/filesystem.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
// libv
#include <libv/read_file.hpp>
#include <libv/timer.hpp>
#include <libv/worker_thread.hpp>
// std
#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
// pro
#include <libv/gl/gl.hpp>
#include <libv/gl/log.hpp>
#include <libv/gl/program.hpp>
#include <libv/gl/shader.hpp>
#include <libv/gl/texture.hpp>
#include <libv/gl/vertex_buffer.hpp>

// -------------------------------------------------------------------------------------------------

float angle = 0;

std::atomic_bool running{true};

// -------------------------------------------------------------------------------------------------
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 900

#define CHECK_GLEW_SUPPORT(ext) LIBV_LOG_INFO("%-46s %s", #ext, glewIsSupported(#ext) ? "[ SUPPORTED ]" : "[UNSUPPORTED]")

void initGLEW() {
	if (GLenum err = glewInit() != GLEW_OK)
		LIBV_LOG_ERROR("Failed to initialize glew: %s", (const char*) glewGetErrorString(err));

	LIBV_LOG_INFO("GL Vendor: %s", (const char*) glGetString(GL_VENDOR));
	LIBV_LOG_INFO("GL Renderer: %s", (const char*) glGetString(GL_RENDERER));
	LIBV_LOG_INFO("GL Version: %s", (const char*) glGetString(GL_VERSION));

	CHECK_GLEW_SUPPORT(GL_VERSION_3_3);
	CHECK_GLEW_SUPPORT(GL_VERSION_4_5);
	CHECK_GLEW_SUPPORT(GL_ARB_direct_state_access);
	CHECK_GLEW_SUPPORT(GL_ARB_draw_elements_base_vertex);
	CHECK_GLEW_SUPPORT(GL_ARB_gpu_shader_fp64);
	CHECK_GLEW_SUPPORT(GL_ARB_sampler_objects);
	CHECK_GLEW_SUPPORT(GL_ARB_vertex_attrib_64bit);
	CHECK_GLEW_SUPPORT(GL_ARB_vertex_attrib_binding);

	checkGL();
}

void initGL() {
	glEnable(GL_DEPTH_TEST); //Depth
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Alpha Type
	glEnable(GL_BLEND); //Alpha
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW); //Counter clockwise polys only

	glPolygonMode(GL_FRONT_AND_BACK, true ? GL_FILL : GL_LINE);
	checkGL();
}

void initGLSL() {
	checkGL();
}

static void error_callback(int code, const char* description) {
	LIBV_LOG_ERROR("GLFW %d: %s", code, description);
}

// -------------------------------------------------------------------------------------------------
struct Example {
	libv::gl::GL gl;

	libv::gl::AttributeFixLocation<glm::vec3> attributePosition;
	libv::gl::AttributeFixLocation<glm::vec4> attributeColor;
	libv::gl::AttributeFixLocation<glm::vec2> attributeTex0;

	libv::gl::GuardedShader shaderTest0Frag;
	libv::gl::GuardedShader shaderTest0Vert;
	libv::gl::GuardedProgram programTest0;
	libv::gl::Uniform<glm::mat4> uniformTest0MVPmat;

	libv::gl::GuardedShader shaderTest1Frag;
	libv::gl::GuardedShader shaderTest1Vert;
	libv::gl::GuardedProgram programTest1;
	libv::gl::Uniform<glm::mat4> uniformTest1MVPmat;
	libv::gl::Uniform<libv::gl::TextureChannel> uniformTest1TextureDiffuseSampler;

	libv::gl::GuardedShader shaderTest2Frag;
	libv::gl::GuardedShader shaderTest2Vert;
	libv::gl::GuardedProgram programTest2;
	libv::gl::Uniform<glm::mat4> uniformTest2MVPmat;
	libv::gl::Uniform<glm::mat4> uniformTest2Mmat;
	libv::gl::Uniform<glm::vec3> uniformTest2EyePosW;
	libv::gl::Uniform<libv::gl::TextureChannel> uniformTest2TextureSkySampler;

	libv::gl::GuardedShader shaderDepthFrag;
	libv::gl::GuardedShader shaderDepthVert;
	libv::gl::GuardedProgram programDepth;
	libv::gl::Uniform<glm::mat4> uniformDepthMVPmat;

	libv::gl::VertexBuffer bufferVertexData;
	libv::gl::VertexBuffer bufferVertexIndices;
	libv::gl::VertexArray vertexArray;

	libv::gl::Texture2DGuard texture0;
	libv::gl::TextureCubeGuard textureSky;

	struct Vertex {
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 uv;

		Vertex(glm::vec3 position, glm::vec4 color, glm::vec2 uv) :
			position(position), color(color), uv(uv) { }
	};

	Example() {
		LIBV_LOG_INFO("%-46s [ %9d ]", "GL_MAX_UNIFORM_BLOCK_SIZE", gl.getMaxUniformBlockSize());
		LIBV_LOG_INFO("%-46s [ %9d ]", "GL_MAX_UNIFORM_BUFFER_BINDINGS", gl.getMaxUniformBufferBindings());
		LIBV_LOG_INFO("%-46s [ %9d ]", "GL_MAX_VERTEX_ATTRIBS", gl.getMaxVertexAttribs());
		LIBV_LOG_INFO("%-46s [ %9d ]", "GL_MAX_VERTEX_UNIFORM_COMPONENTS", gl.getMaxVertexUniformComponents());
		LIBV_LOG_INFO("%-46s [ %9d ]", "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS", gl.getMaxCombinedTextureImageUnits());

		Vertex dataVertex[]{
			Vertex{glm::vec3(-1.f, -1.f, 0.f), glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec2(0.f, 0.f)},
			Vertex{glm::vec3(+1.f, -1.f, 0.f), glm::vec4(1.f, 1.f, 0.f, 1.f), glm::vec2(1.f, 0.f)},
			Vertex{glm::vec3(+1.f, +1.f, 0.f), glm::vec4(0.f, 1.f, 1.f, 1.f), glm::vec2(1.f, 1.f)},
			Vertex{glm::vec3(-1.f, +1.f, 0.f), glm::vec4(0.f, 0.f, 1.f, 1.f), glm::vec2(0.f, 1.f)}
		};

		unsigned int dataIndices[]{0, 1, 2, 0, 2, 3};

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

		attributePosition = 0;
		attributeColor = 2;
		attributeTex0 = 8;

		shaderTest0Frag.createCompile(libv::gl::ShaderType::Fragment, libv::read_file("res/shader/test0.fs"));
		shaderTest0Vert.createCompile(libv::gl::ShaderType::Vertex, libv::read_file("res/shader/test0.vs"));
		programTest0.link(shaderTest0Frag, shaderTest0Vert);
		programTest0.assign(uniformTest0MVPmat, "MVPmat");

		shaderTest1Frag.createCompile(libv::gl::ShaderType::Fragment, libv::read_file("res/shader/test1.fs"));
		shaderTest1Vert.createCompile(libv::gl::ShaderType::Vertex, libv::read_file("res/shader/test1.vs"));
		programTest1.link(shaderTest1Frag, shaderTest1Vert);
		programTest1.assign(uniformTest1MVPmat, "MVPmat");
		programTest1.assign(uniformTest1TextureDiffuseSampler, "textureDiffuseSampler");

		shaderTest2Frag.createCompile(libv::gl::ShaderType::Fragment, libv::read_file("res/shader/test2.fs"));
		shaderTest2Vert.createCompile(libv::gl::ShaderType::Vertex, libv::read_file("res/shader/test2.vs"));
		programTest2.link(shaderTest2Frag, shaderTest2Vert);
		programTest2.assign(uniformTest2MVPmat, "MVPmat");
		programTest2.assign(uniformTest2Mmat, "Mmat");
		programTest2.assign(uniformTest2EyePosW, "eyePosW");
		programTest2.assign(uniformTest2TextureSkySampler, "textureSkySampler");

		shaderDepthFrag.createCompile(libv::gl::ShaderType::Fragment, libv::read_file("res/shader/depth.fs"));
		shaderDepthVert.createCompile(libv::gl::ShaderType::Vertex, libv::read_file("res/shader/depth.vs"));
		programDepth.link(shaderDepthFrag, shaderDepthVert);
		programDepth.assign(uniformDepthMVPmat, "MVPmat");

		bufferVertexData.createData(&dataVertex[0], sizeof(dataVertex), libv::gl::BufferUsage::StaticDraw);
		bufferVertexIndices.createData(&dataIndices[0], sizeof(dataIndices), libv::gl::BufferUsage::StaticDraw);

		vertexArray.create();
		vertexArray.bindAttribute(bufferVertexData, attributePosition, sizeof(Vertex), offsetof(Vertex, position), false);
		vertexArray.bindAttribute(bufferVertexData, attributeColor, sizeof(Vertex), offsetof(Vertex, color), false);
		vertexArray.bindAttribute(bufferVertexData, attributeTex0, sizeof(Vertex), offsetof(Vertex, uv), false);
		vertexArray.bindElements(bufferVertexIndices);

		texture0.create();
		texture0.bind();
		texture0.storage2D(1, libv::gl::InternalFormat::RGBA8, 2, 2);
		texture0.subImage2D(0, 0, 0, libv::gl::Format::RGBA, libv::gl::DataType::UnsignedByte, 2, 2, dataTexture);
		texture0.setMinFilter(libv::gl::MinFilter::Linear);
		texture0.setMagFilter(libv::gl::MagFilter::Linear);
		texture0.setWrapS(libv::gl::Wrap::ClampToEdge);
		texture0.setWrapT(libv::gl::Wrap::ClampToEdge);
		texture0.unbind();

		textureSky.create();
		textureSky.bind();
		textureSky.storage2D(1, libv::gl::InternalFormat::RGBA8, 2, 2);
		textureSky.subImage2D(libv::gl::CubeSide::PositiveX, 0, 0, 0, libv::gl::Format::RGBA, libv::gl::DataType::UnsignedInt_8_8_8_8, 2, 2, dataTextureSkyX1);
		textureSky.subImage2D(libv::gl::CubeSide::NegativeX, 0, 0, 0, libv::gl::Format::RGBA, libv::gl::DataType::UnsignedInt_8_8_8_8, 2, 2, dataTextureSkyX0);
		textureSky.subImage2D(libv::gl::CubeSide::PositiveY, 0, 0, 0, libv::gl::Format::RGBA, libv::gl::DataType::UnsignedInt_8_8_8_8, 2, 2, dataTextureSkyY1);
		textureSky.subImage2D(libv::gl::CubeSide::NegativeY, 0, 0, 0, libv::gl::Format::RGBA, libv::gl::DataType::UnsignedInt_8_8_8_8, 2, 2, dataTextureSkyY0);
		textureSky.subImage2D(libv::gl::CubeSide::PositiveZ, 0, 0, 0, libv::gl::Format::RGBA, libv::gl::DataType::UnsignedInt_8_8_8_8, 2, 2, dataTextureSkyZ1);
		textureSky.subImage2D(libv::gl::CubeSide::NegativeZ, 0, 0, 0, libv::gl::Format::RGBA, libv::gl::DataType::UnsignedInt_8_8_8_8, 2, 2, dataTextureSkyZ0);
		textureSky.unbind();
	}

	void render() {
		gl.clearColor(0.236f, 0.311f, 0.311f, 1.f);
		gl.clear();

		auto pStackGuard = gl.projection.pushGuard();
		auto vStackGuard = gl.view.pushGuard();
		auto mStackGuard = gl.model.pushGuard();

		angle += 0.5f;

		gl.projection.setToPerspective(1.f, 1.f * WINDOW_WIDTH / WINDOW_HEIGHT, 1.f, 1000.f);
		gl.view.setToLookAt(glm::vec3(5.f, 3.f, 5.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		gl.view.rotate(angle / 90, glm::vec3(0, 1, 0));
		gl.model.identity();

		// -----------------------------------------------------------------------------------------

		// TODO P4: Binding a texture to an uniform instead of TextureType sounds like a good idea.

		gl.model.translate(-1, -1, +0);

		{
			programTest0.use();
			uniformTest0MVPmat = gl.mvp();
			gl.drawElements(vertexArray, libv::gl::Primitive::Triangles, 6, 0);
		}

		gl.model.translate(+0, +2, +0);

		{
			programTest1.use();
			gl.activeTexture(libv::gl::TextureChannel::diffuse);
			auto tBindGuard = texture0.bindGuard();
			uniformTest1MVPmat = gl.mvp();
			uniformTest1TextureDiffuseSampler = libv::gl::TextureChannel::diffuse;
			gl.drawElements(vertexArray, libv::gl::Primitive::Triangles, 6, 0);
		}

		gl.model.translate(+2, +0, +0);

		{
			programTest2.use();
			gl.activeTexture(libv::gl::TextureChannel::sky);
			auto sBindGuard = textureSky.bindGuard();
			uniformTest2MVPmat = gl.mvp();
			uniformTest2Mmat = gl.model;
			uniformTest2EyePosW = gl.view.eye();
			uniformTest2TextureSkySampler = libv::gl::TextureChannel::sky;
			gl.drawElements(vertexArray, libv::gl::Primitive::Triangles, 6, 0);
		}

		checkGL();
	}
};

// -------------------------------------------------------------------------------------------------

void init() {
	initGLEW();
	initGL();
	initGLSL();
}

int main(void) {
	std::cout << libv::log;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		LIBV_LOG_ERROR("Failed to initialize GLFW.");
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	GLFWwindow* window;

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		LIBV_LOG_ERROR("Failed to create GLFW window.");
		exit(EXIT_FAILURE);
	}
	glfwSetWindowPos(window, 200, 200);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	init();

	{
		Example example;

		libv::Timer timer;
		size_t time = 0, i = 0;


		while (!glfwWindowShouldClose(window)) {
			checkGL();
			example.render();

			glfwSwapBuffers(window);
			glfwPollEvents();

			i++;
			time += timer.time().count();
			if (time > 1000000000) {
				LIBV_LOG_INFO("FPS: %d", i);
				i = 0;
				time -= 1000000000;
			}
		}
	}
	glfwMakeContextCurrent(nullptr);
	glfwTerminate();

	return 0;
}

//	std::this_thread::sleep_for(std::chrono::seconds(2));

//libv::gl::renderCube(-1.0f, 0, 0, 0.4f);
//libv::gl::renderCube(+0.0f, 0, 0, 0.6f);
//libv::gl::renderCube(+1.4f, 0, 0, 0.8f);