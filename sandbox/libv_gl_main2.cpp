// File: vgl2_main.cpp, Created on 2016.07.30. at 09:27, Author: Vader

// ext
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// libv
#include <libv/utility/read_file.hpp>
#include <libv/utility/timer.hpp>
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
#include <libv/gl/image.hpp>


// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_sandbox{libv::logger, "libv.sandbox"};

constexpr uint32_t WINDOW_HEIGHT = 600;
constexpr uint32_t WINDOW_WIDTH = 900;

void checkGLSupport(const char* ext) {
	log_sandbox.info("{:46} [{}]", ext, glewIsSupported(ext) ? " SUPPORTED " : "UNSUPPORTED");
}

// -------------------------------------------------------------------------------------------------

struct Sandbox {
	float angle = 0;

	libv::gl::GL gl;

	libv::gl::AttributeFixLocation<libv::vec3f> attributePosition;
	libv::gl::AttributeFixLocation<libv::vec2f> attributeUV;

	libv::gl::GuardedShader shaderTest1Frag;
	libv::gl::GuardedShader shaderTest1Vert;
	libv::gl::GuardedProgram programTest1;
	libv::gl::Uniform_mat4f uniformTest1MVPmat;
	libv::gl::Uniform_texture uniformTest1TextureDiffuseSampler;

	libv::gl::GuardedShader shaderTest2Frag;
	libv::gl::GuardedShader shaderTest2Vert;
	libv::gl::GuardedProgram programTest2;
	libv::gl::Uniform_mat4f uniformTest2MVPmat;
	libv::gl::Uniform_mat4f uniformTest2Mmat;
	libv::gl::Uniform_vec3f uniformTest2EyePosW;
	libv::gl::Uniform_texture uniformTest2TextureSkySampler;

	libv::gl::VertexBuffer bufferVertexData;
	libv::gl::VertexBuffer bufferVertexIndices;
	libv::gl::VertexArray vertexArray;

	libv::gl::Texture2DGuard texturePlane;
	libv::gl::TextureCubeGuard textureSky;

	struct Vertex {
		libv::vec3f position;
		libv::vec2f uv;

		Vertex(libv::vec3f position, libv::vec2f uv) :
			position(position), uv(uv) { }
	};

	Sandbox() {
		log_sandbox.info("GL Vendor: {}", glGetString(GL_VENDOR));
		log_sandbox.info("GL Renderer: {}", glGetString(GL_RENDERER));
		log_sandbox.info("GL Version: {}", glGetString(GL_VERSION));

		checkGLSupport("GL_VERSION_3_3");
		checkGLSupport("GL_VERSION_4_5");
		checkGLSupport("GL_ARB_direct_state_access");
		checkGLSupport("GL_ARB_draw_elements_base_vertex");
		checkGLSupport("GL_ARB_gpu_shader_fp64");
		checkGLSupport("GL_ARB_sampler_objects");
		checkGLSupport("GL_ARB_vertex_attrib_64bit");
		checkGLSupport("GL_ARB_vertex_attrib_binding");

		libv::gl::checkGL();

		log_sandbox.info("{:46} [ {:9} ]", "GL_MAX_UNIFORM_BLOCK_SIZE", gl.getMaxUniformBlockSize());
		log_sandbox.info("{:46} [ {:9} ]", "GL_MAX_UNIFORM_BUFFER_BINDINGS", gl.getMaxUniformBufferBindings());
		log_sandbox.info("{:46} [ {:9} ]", "GL_MAX_VERTEX_ATTRIBS", gl.getMaxVertexAttribs());
		log_sandbox.info("{:46} [ {:9} ]", "GL_MAX_VERTEX_UNIFORM_COMPONENTS", gl.getMaxVertexUniformComponents());
		log_sandbox.info("{:46} [ {:9} ]", "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS", gl.getMaxCombinedTextureImageUnits());
		log_sandbox.info("{:46} [ {:9} ]", "GL_MAX_TEXTURE_SIZE", gl.getMaxTextureSize());

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
		gl.polygonMode(true ? libv::gl::Mode::Fill : libv::gl::Mode::Line);

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

		attributePosition = 0;
		attributeUV = 8;

		shaderTest2Frag.createCompile(libv::gl::ShaderType::Fragment, libv::read_file_or_throw("res/shader/test2.fs"));
		shaderTest2Vert.createCompile(libv::gl::ShaderType::Vertex, libv::read_file_or_throw("res/shader/test2.vs"));
		programTest2.link(shaderTest2Frag, shaderTest2Vert);
		programTest2.assign(uniformTest2MVPmat, "MVPmat");
		programTest2.assign(uniformTest2Mmat, "Mmat");
		programTest2.assign(uniformTest2EyePosW, "eyePosW");
		programTest2.assign(uniformTest2TextureSkySampler, "textureSkySampler");

		shaderTest1Frag.createCompile(libv::gl::ShaderType::Fragment, libv::read_file_or_throw("res/shader/test1.fs"));
		shaderTest1Vert.createCompile(libv::gl::ShaderType::Vertex, libv::read_file_or_throw("res/shader/test1.vs"));
		programTest1.link(shaderTest1Frag, shaderTest1Vert);
		programTest1.assign(uniformTest1MVPmat, "MVPmat");
		programTest1.assign(uniformTest1TextureDiffuseSampler, "textureDiffuseSampler");

		bufferVertexData.createData(&dataVertex[0], sizeof(dataVertex), libv::gl::BufferUsage::StaticDraw);
		bufferVertexIndices.createData(&dataIndices[0], sizeof(dataIndices), libv::gl::BufferUsage::StaticDraw);

		vertexArray.create();
		{
			auto vao_guard = vertexArray.bind_guard();
			vertexArray.bindAttribute(bufferVertexData, attributePosition, sizeof(Vertex), offsetof(Vertex, position), false);
			vertexArray.bindAttribute(bufferVertexData, attributeUV, sizeof(Vertex), offsetof(Vertex, uv), false);
			vertexArray.bindElements(bufferVertexIndices);
		}

		auto dataPlane = libv::read_file_or_throw("res/texture/6poly_metal_01_diffuse.png");
		auto imagePlane = libv::gl::load_image_or_throw(dataPlane);
		texturePlane.adopt(imagePlane.createTexture());

		auto dataSky = libv::read_file_or_throw("res/texture/sky/merged2.dds");
		auto imageSky = libv::gl::load_image_or_throw(dataSky);
		textureSky.adopt(imageSky.createTexture());

		programTest1.use();
		uniformTest1TextureDiffuseSampler = libv::gl::TextureChannel::diffuse;

		programTest2.use();
		uniformTest2TextureSkySampler = libv::gl::TextureChannel::sky;

		libv::gl::checkGL();
	}

	void render() {
		gl.clearColor(0.236f, 0.311f, 0.311f, 1.f);
		gl.clear();

		const auto pStackGuard = gl.projection.push_guard();
		const auto vStackGuard = gl.view.push_guard();
		const auto mStackGuard = gl.model.push_guard();

		angle += 0.5f;

		gl.projection = libv::perspective(1.f, 1.f * WINDOW_WIDTH / WINDOW_HEIGHT, 1.f, 1000.f);
		gl.view = libv::lookAt(libv::vec3f(5.f, 3.f, 5.f), libv::vec3f(0.f, 0.f, 0.f), libv::vec3f(0.f, 1.f, 0.f));
		gl.view.rotate(angle / 90.f, libv::vec3f(0.f, 1.f, 0.f));
		gl.model = libv::identity<4, float>();

		// Draw Sky
		{
			auto eye = gl.eye();

			auto gurad_modelStack = gl.model.push_guard();
			gl.model.translate(eye);
			gl.model.scale(libv::vec3f(3.f, 3.f, 3.f));

			auto guard_depth = gl.capability.depthTest.disable_guard();
			auto guard_frontFace = gl.frontFace.cw_guard(); // Cheat for the sake of sandbox
			programTest2.use();
			gl.activeTexture(libv::gl::TextureChannel::sky);
			auto guard_textureBind = textureSky.bindGuard();
			uniformTest2MVPmat = gl.mvp();
			uniformTest2Mmat = gl.model;
			uniformTest2EyePosW = eye;

			auto vao_guard = vertexArray.bind_guard();
			vertexArray.drawElements(libv::gl::Primitive::Triangles, 36, 0);
		}

		// Draw world
		{
			programTest1.use();
			gl.activeTexture(libv::gl::TextureChannel::diffuse);
			auto guard_textureBind = texturePlane.bindGuard();
			uniformTest1MVPmat = gl.mvp();

			auto vao_guard = vertexArray.bind_guard();
			vertexArray.drawElements(libv::gl::Primitive::Triangles, 36, 0);
		}

		libv::gl::checkGL();
	}
};

// Runner ------------------------------------------------------------------------------------------

auto running = std::atomic_bool{true};

int main(void) {
	std::cout << libv::logger;

	glfwSetErrorCallback([](int code, const char* description) {
		log_sandbox.error("GLFW {}: {}", code, description);
	});

	if (!glfwInit()) {
		log_sandbox.error("Failed to initialize GLFW.");
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window;

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		log_sandbox.error("Failed to create GLFW window.");
		exit(EXIT_FAILURE);
	}
	glfwSetWindowPos(window, 200, 200);

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, [](GLFWwindow*, int key, int, int, int) {
		if (key == GLFW_KEY_ESCAPE)
			running = false;
	});
	glfwSwapInterval(1);

	// TODO P1: Make same fix for gl3 sandbox
	// TODO P1: Make glew init part of libv.gl
	if (GLenum err = glewInit() != GLEW_OK)
		log_sandbox.error("Failed to initialize glew: {}", glewGetErrorString(err));

	libv::gl::checkGL();

	try {
		Sandbox sandbox;

		std::chrono::nanoseconds time_outside;
		std::chrono::nanoseconds time_render;
		std::chrono::nanoseconds time_swap;
		std::chrono::nanoseconds time_poll;

		libv::Timer timer;
		libv::Timer print_timer;
		size_t i = 0;

		while (running && !glfwWindowShouldClose(window)) {
			libv::gl::checkGL();
			time_outside += timer.time();

			sandbox.render();
			time_render += timer.time();

			glfwSwapBuffers(window);
			time_swap += timer.time();

			glfwPollEvents();
			time_poll += timer.time();

			i++;
			if (print_timer.elapsed() > std::chrono::seconds(1)) {
				print_timer.adjust(std::chrono::seconds(1));

				log_sandbox.info("Frames: {}, Poll: {:7.3f}μs, Render: {:7.3f}μs, Other: {:7.3f}μs, Swap: {:7.3f}μs, Sum: {:7.3f}μs",
						i,
						time_poll.count() / 1000.f / i,
						time_render.count() / 1000.f / i,
						time_outside.count() / 1000.f / i,
						time_swap.count() / 1000.f / i,
						(time_outside + time_render + time_swap + time_poll).count() / 1000.f / i
				);
				i = 0;

				time_outside = time_outside.zero();
				time_render = time_render.zero();
				time_swap = time_swap.zero();
				time_poll = time_poll.zero();
			}
		}
	} catch (const std::exception& e) {
		log_sandbox.fatal("Caught exception: {}", e.what());
	}

	libv::gl::checkGL();
	glfwMakeContextCurrent(nullptr);
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

// -------------------------------------------------------------------------------------------------
