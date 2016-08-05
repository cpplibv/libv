// File: vgl2_main.cpp, Created on 2016.07.30. at 09:27, Author: Vader

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

#define CHECK_GLEW_SUPPORT(ext) LIBV_LIBV_INFO("%-46s %s", #ext, glewIsSupported(#ext) ? "[ SUPPORTED ]" : "[UNSUPPORTED]")

void initGLEW() {
	if (GLenum err = glewInit() != GLEW_OK)
		LIBV_LIBV_ERROR("Failed to initialize glew: %s", (const char*) glewGetErrorString(err));

	LIBV_LIBV_INFO("GL Vendor: %s", (const char*) glGetString(GL_VENDOR));
	LIBV_LIBV_INFO("GL Renderer: %s", (const char*) glGetString(GL_RENDERER));
	LIBV_LIBV_INFO("GL Version: %s", (const char*) glGetString(GL_VERSION));

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

	glEnable(GL_TEXTURE_2D);

	glPolygonMode(GL_FRONT_AND_BACK, true ? GL_FILL : GL_LINE);
	checkGL();
}

void initGLSL() {
	checkGL();
}

static void error_callback(int code, const char* description) {
	LIBV_LIBV_ERROR("GLFW %d: %s", code, description);
}

// -------------------------------------------------------------------------------------------------
struct Example {
	libv::gl::GL gl;

	libv::gl::AttributeFixLocation<glm::vec3> attributePosition;
	libv::gl::AttributeFixLocation<glm::vec2> attributeUV;

	libv::gl::Shader shaderTest1Frag;
	libv::gl::Shader shaderTest1Vert;
	libv::gl::Program programTest1;
	libv::gl::Uniform<glm::mat4> uniformTest1MVPmat;
	libv::gl::Uniform<libv::gl::TextureChannel> uniformTest1TextureDiffuseSampler;

	libv::gl::Shader shaderTest2Frag;
	libv::gl::Shader shaderTest2Vert;
	libv::gl::Program programTest2;
	libv::gl::Uniform<glm::mat4> uniformTest2MVPmat;
	libv::gl::Uniform<glm::mat4> uniformTest2Mmat;
	libv::gl::Uniform<glm::vec3> uniformTest2EyePosW;
	libv::gl::Uniform<libv::gl::TextureChannel> uniformTest2TextureSkySampler;

	libv::gl::VertexBuffer bufferVertexData;
	libv::gl::VertexBuffer bufferVertexIndices;
	libv::gl::VertexArray vertexArray;

	libv::gl::Texture2DGuard texturePlane;
	libv::gl::TextureCubeGuard textureSky;

	struct Vertex {
		glm::vec3 position;
		glm::vec2 uv;

		Vertex(glm::vec3 position, glm::vec2 uv) :
			position(position), uv(uv) { }
	};

	Example() {
		Vertex dataVertex[]{
			Vertex{glm::vec3(-1.f, -1.f, +1.f), glm::vec2(0.f, 0.f)},
			Vertex{glm::vec3(+1.f, -1.f, +1.f), glm::vec2(1.f, 0.f)},
			Vertex{glm::vec3(+1.f, +1.f, +1.f), glm::vec2(1.f, 1.f)},
			Vertex{glm::vec3(-1.f, +1.f, +1.f), glm::vec2(0.f, 1.f)},

			Vertex{glm::vec3(+1.f, +1.f, -1.f), glm::vec2(0.f, 0.f)},
			Vertex{glm::vec3(+1.f, -1.f, -1.f), glm::vec2(1.f, 0.f)},
			Vertex{glm::vec3(-1.f, -1.f, -1.f), glm::vec2(1.f, 1.f)},
			Vertex{glm::vec3(-1.f, +1.f, -1.f), glm::vec2(0.f, 1.f)},

			Vertex{glm::vec3(+1.f, +1.f, +1.f), glm::vec2(1.f, 1.f)},
			Vertex{glm::vec3(+1.f, +1.f, -1.f), glm::vec2(1.f, 0.f)},
			Vertex{glm::vec3(-1.f, +1.f, -1.f), glm::vec2(0.f, 0.f)},
			Vertex{glm::vec3(-1.f, +1.f, +1.f), glm::vec2(0.f, 1.f)},

			Vertex{glm::vec3(-1.f, -1.f, -1.f), glm::vec2(1.f, 1.f)},
			Vertex{glm::vec3(+1.f, -1.f, -1.f), glm::vec2(1.f, 0.f)},
			Vertex{glm::vec3(+1.f, -1.f, +1.f), glm::vec2(0.f, 0.f)},
			Vertex{glm::vec3(-1.f, -1.f, +1.f), glm::vec2(0.f, 1.f)},

			Vertex{glm::vec3(+1.f, -1.f, -1.f), glm::vec2(0.f, 0.f)},
			Vertex{glm::vec3(+1.f, +1.f, -1.f), glm::vec2(1.f, 0.f)},
			Vertex{glm::vec3(+1.f, +1.f, +1.f), glm::vec2(1.f, 1.f)},
			Vertex{glm::vec3(+1.f, -1.f, +1.f), glm::vec2(0.f, 1.f)},

			Vertex{glm::vec3(-1.f, +1.f, +1.f), glm::vec2(0.f, 0.f)},
			Vertex{glm::vec3(-1.f, +1.f, -1.f), glm::vec2(1.f, 0.f)},
			Vertex{glm::vec3(-1.f, -1.f, -1.f), glm::vec2(1.f, 1.f)},
			Vertex{glm::vec3(-1.f, -1.f, +1.f), glm::vec2(0.f, 1.f)}
		};

		uint32_t dataIndices[]{
			 0,  1,  2,  0,  2,  3,
			 4,  5,  6,  4,  6,  7,
			 8,  9, 10,  8, 10, 11,
			12, 13, 14, 12, 14, 15,
			16, 17, 18, 16, 18, 19,
			20, 21, 22, 20, 22, 23
		};

		attributePosition = 0;
		attributeUV = 8;

		shaderTest2Frag.createCompile(libv::readFileText("res/shader/test2.fs"), libv::gl::ShaderType::Fragment);
		shaderTest2Vert.createCompile(libv::readFileText("res/shader/test2.vs"), libv::gl::ShaderType::Vertex);
		programTest2.link(shaderTest2Frag, shaderTest2Vert);
		programTest2.assign(uniformTest2MVPmat, "MVPmat");
		programTest2.assign(uniformTest2Mmat, "Mmat");
		programTest2.assign(uniformTest2EyePosW, "eyePosW");
		programTest2.assign(uniformTest2TextureSkySampler, "textureSkySampler");

		shaderTest1Frag.createCompile(libv::readFileText("res/shader/test1.fs"), libv::gl::ShaderType::Fragment);
		shaderTest1Vert.createCompile(libv::readFileText("res/shader/test1.vs"), libv::gl::ShaderType::Vertex);
		programTest1.link(shaderTest1Frag, shaderTest1Vert);
		programTest1.assign(uniformTest1MVPmat, "MVPmat");
		programTest1.assign(uniformTest1TextureDiffuseSampler, "textureDiffuseSampler");

		bufferVertexData.data(&dataVertex[0], sizeof(dataVertex), libv::gl::BufferUsage::StaticDraw);
		bufferVertexIndices.data(&dataIndices[0], sizeof(dataIndices), libv::gl::BufferUsage::StaticDraw);

		vertexArray.bindAttribute(bufferVertexData, attributePosition, sizeof(Vertex), offsetof(Vertex, position), false);
		vertexArray.bindAttribute(bufferVertexData, attributeUV, sizeof(Vertex), offsetof(Vertex, uv), false);
		vertexArray.bindElements(bufferVertexIndices);

		auto dataPlane = libv::readFileBinary("res/texture/6poly_metal_01_diffuse.dds");
		texturePlane.createFromDDS(dataPlane.data(), dataPlane.size());
		auto dataSky = libv::readFileBinary("res/texture/sky/merged2.dds");
		textureSky.createFromDDS(dataSky.data(), dataSky.size());

		programTest1.use();
		uniformTest1TextureDiffuseSampler = libv::gl::TextureChannel::diffuse;

		programTest2.use();
		uniformTest2TextureSkySampler = libv::gl::TextureChannel::sky;

		checkGL();
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

		// Draw Sky
		{
			auto mStackGuard = gl.model.pushGuard();
			gl.model.translate(gl.view.eye());
			gl.model.scale(3, 3, 3);

			auto dCapabilityGuard = gl.disableGuard(libv::gl::Capability::DepthTest);
			gl.frontFaceCW(); // Cheat for the sake of sandbox
			programTest2.use();
			gl.activeTexture(libv::gl::TextureChannel::sky);
			auto sBindGuard = textureSky.bindGuard();
			uniformTest2MVPmat = gl.mvp();
			uniformTest2Mmat = gl.model;
			uniformTest2EyePosW = gl.view.eye();

			gl.drawElements(vertexArray, libv::gl::Primitive::Triangles, 36, 0);

			gl.frontFaceCCW(); // Cheat for the sake of sandbox
		}

		// Draw world
		{
			programTest1.use();
			gl.activeTexture(libv::gl::TextureChannel::diffuse);
			auto pBindGuard = texturePlane.bindGuard();
			uniformTest1MVPmat = gl.mvp();

			gl.drawElements(vertexArray, libv::gl::Primitive::Triangles, 36, 0);

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
		LIBV_LIBV_ERROR("Failed to initialize GLFW.");
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
		LIBV_LIBV_ERROR("Failed to create GLFW window.");
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
				LIBV_LIBV_INFO("FPS: %d", i);
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
