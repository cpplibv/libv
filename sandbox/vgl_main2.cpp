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
#include <libv/gl/uniform.hpp>
#include <libv/gl/vertex_buffer.hpp>

// -------------------------------------------------------------------------------------------------

float angle = 0;

std::atomic_bool running{true};

// -------------------------------------------------------------------------------------------------
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 900

#define checkGLEWSupport(ext) LIBV_LIBV_INFO("GLEW: %-40s %s", #ext, glewIsSupported(#ext) ? "[ SUPPORTED ]" : "[UNSUPPORTED]")

void initGLEW() {
	if (GLenum err = glewInit() != GLEW_OK)
		LIBV_LIBV_ERROR("Failed to initialize glew: %s", (const char*) glewGetErrorString(err));

	LIBV_LIBV_INFO("GL Vendor: %s", (const char*) glGetString(GL_VENDOR));
	LIBV_LIBV_INFO("GL Renderer: %s", (const char*) glGetString(GL_RENDERER));
	LIBV_LIBV_INFO("GL Version: %s", (const char*) glGetString(GL_VERSION));

	checkGLEWSupport(GL_VERSION_3_3);
	checkGLEWSupport(GL_VERSION_4_5);
	checkGLEWSupport(GL_ARB_direct_state_access);
	checkGLEWSupport(GL_ARB_draw_elements_base_vertex);
	checkGLEWSupport(GL_ARB_gpu_shader_fp64);
	checkGLEWSupport(GL_ARB_sampler_objects);
	checkGLEWSupport(GL_ARB_vertex_attrib_64bit);
	checkGLEWSupport(GL_ARB_vertex_attrib_binding);

	checkGL();
}

void initGL() {
	glEnable(GL_DEPTH_TEST); //Depth
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Alpha Type
	glEnable(GL_BLEND); //Alpha
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
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

	libv::gl::Shader shaderTest2Frag;
	libv::gl::Shader shaderTest2Vert;
	libv::gl::Program programTest2;
	libv::gl::Uniform<glm::mat4> uniformTest2MVPmat;
	libv::gl::Uniform<glm::mat4> uniformTest2Mmat;
	libv::gl::Uniform<glm::vec3> uniformTest2EyePosW;
	libv::gl::Uniform<libv::gl::TextureChannel> uniformTest2TextureSkySampler;
	libv::gl::Attribute<glm::vec3> attributeTest2Position;

	libv::gl::VertexBuffer bufferVertexData;
	libv::gl::VertexBuffer bufferVertexIndices;
	libv::gl::VertexArray vertexArray;

	libv::gl::Texture textureSky;

	struct Vertex {
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 uv;

		Vertex(glm::vec3 position, glm::vec4 color, glm::vec2 uv) :
			position(position), color(color), uv(uv) { }
	};

	Example() {
		Vertex dataVertex[]{
			Vertex{glm::vec3(-1.f, -1.f, 0.f), glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec2(0.f, 0.f)},
			Vertex{glm::vec3(+1.f, -1.f, 0.f), glm::vec4(1.f, 1.f, 0.f, 1.f), glm::vec2(1.f, 0.f)},
			Vertex{glm::vec3(+1.f, +1.f, 0.f), glm::vec4(0.f, 1.f, 1.f, 1.f), glm::vec2(1.f, 1.f)},
			Vertex{glm::vec3(-1.f, +1.f, 0.f), glm::vec4(0.f, 0.f, 1.f, 1.f), glm::vec2(0.f, 1.f)}
		};

		unsigned int dataIndices[]{0, 2, 1, 0, 3, 2};

		shaderTest2Frag.createCompile(libv::readFileText("res/shader/test2.fs"), libv::gl::ShaderType::Fragment);
		shaderTest2Vert.createCompile(libv::readFileText("res/shader/test2.vs"), libv::gl::ShaderType::Vertex);
		programTest2.link(shaderTest2Frag, shaderTest2Vert);
		programTest2.bind(uniformTest2MVPmat, "MVPmat");
		programTest2.bind(uniformTest2Mmat, "Mmat");
		programTest2.bind(uniformTest2EyePosW, "eyePosW");
		programTest2.bind(uniformTest2TextureSkySampler, "textureSkySampler");
		programTest2.bind(attributeTest2Position, "vertexPos");

		bufferVertexData.data(&dataVertex[0], sizeof(dataVertex), libv::gl::BufferUsage::StaticDraw);
		bufferVertexIndices.data(&dataIndices[0], sizeof(dataIndices), libv::gl::BufferUsage::StaticDraw);

		vertexArray.bindAttribute(bufferVertexData, attributeTest0Position, sizeof(Vertex), offsetof(Vertex, position), false);
		vertexArray.bindAttribute(bufferVertexData, attributeTest0Color, sizeof(Vertex), offsetof(Vertex, color), false);
		vertexArray.bindElements(bufferVertexIndices);

		textureSky.storage2D(libv::gl::TextureBindTarget::CubeMap, 1, libv::gl::InternalFormat::RGBA8, 2, 2);
		textureSky.subImage2D(libv::gl::CubeSide::PositiveX, 0, 0, 0, libv::gl::Format::RGBA, libv::gl::DataType::UnsignedInt_8_8_8_8, 2, 2, dataTextureSkyX1);
		textureSky.subImage2D(libv::gl::CubeSide::NegativeX, 0, 0, 0, libv::gl::Format::RGBA, libv::gl::DataType::UnsignedInt_8_8_8_8, 2, 2, dataTextureSkyX0);
		textureSky.subImage2D(libv::gl::CubeSide::PositiveY, 0, 0, 0, libv::gl::Format::RGBA, libv::gl::DataType::UnsignedInt_8_8_8_8, 2, 2, dataTextureSkyY1);
		textureSky.subImage2D(libv::gl::CubeSide::NegativeY, 0, 0, 0, libv::gl::Format::RGBA, libv::gl::DataType::UnsignedInt_8_8_8_8, 2, 2, dataTextureSkyY0);
		textureSky.subImage2D(libv::gl::CubeSide::PositiveZ, 0, 0, 0, libv::gl::Format::RGBA, libv::gl::DataType::UnsignedInt_8_8_8_8, 2, 2, dataTextureSkyZ1);
		textureSky.subImage2D(libv::gl::CubeSide::NegativeZ, 0, 0, 0, libv::gl::Format::RGBA, libv::gl::DataType::UnsignedInt_8_8_8_8, 2, 2, dataTextureSkyZ0);
	}

	void render() {
		glClearColor(0.236f, 0.311f, 0.311f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gl.pushMatrixProjection();
		gl.pushMatrixView();
		gl.pushMatrixModel();

		gl.matrixProjection() = glm::perspective(1.f, 1.f * WINDOW_WIDTH / WINDOW_HEIGHT, 1.f, 1000.f);
		gl.matrixView() = glm::lookAt(glm::vec3(5.f, 3.f, 5.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		gl.matrixView() *= glm::rotate(angle / 90, glm::vec3(0, 1, 0));

		angle += 0.5f;

		// ---------------------------------------------------------------------------------------------

		// TODO P4: Binding a texture to an uniform instead of TextureType sounds like a good idea.

		programTest2.use();
		textureSky.bind(libv::gl::TextureChannel::sky);
		uniformTest2MVPmat = gl.matrixMVP();
		uniformTest2Mmat = gl.matrixModel();
		uniformTest2EyePosW = glm::vec3(glm::inverse(gl.matrixView())[3]);
		uniformTest2TextureSkySampler = libv::gl::TextureChannel::sky;
		gl.drawElements(vertexArray, libv::gl::Primitive::Triangles, 6, 0);

		checkGL();

		gl.popMatrixModel();
		gl.popMatrixView();
		gl.popMatrixProjection();

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
			if (time > 1'000'000'000) {
				LIBV_LIBV_INFO("FPS: %d", i);
				i = 0;
				time -= 1'000'000'000;
			}
		}
	}
	glfwMakeContextCurrent(nullptr);
	glfwTerminate();

	return 0;
}

//	std::this_thread::sleep_for(std::chrono::seconds(2));
