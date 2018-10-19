// File: libv_gl_main3.cpp, Created on 2017.11.17. at 22:38, Author: Vader

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
//#include <libv/gl/program.hpp>
//#include <libv/gl/shader.hpp>
//#include <libv/gl/texture.hpp>
//#include <libv/gl/vertex_buffer.hpp>
//#include <libv/gl/image.hpp>

#include <libv/glr/mesh.hpp>
#include <libv/glr/remote.hpp>
#include <libv/glr/vertex.hpp>


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

	libv::glr::Remote remote;

	libv::glr::Mesh boxMesh;

//
//	libv::glr::Shader shaderTest1Fragment;
//	libv::glr::Shader shaderTest1Vertex;
//	libv::glr::Program programTest1;
////	libv::glr::Texture2D texturePlane;
//	libv::glr::Uniform_mat4f uniformTest1MVPmat; // Move into glr:: Program/Queue
////	libv::glr::Uniform_texture uniformTest1TextureDiffuseSampler{programTest1, "textureDiffuseSampler"};


	// Move to glr
	libv::gl::Shader shaderTest1Fragment;
	libv::gl::Shader shaderTest1Vertex;
	libv::gl::Program programTest1;
	libv::gl::Uniform_mat4f uniformTest1MVPmat;

	Sandbox() {
//		log_sandbox.info("{:46} [ {:9} ]", "GL_MAX_UNIFORM_BLOCK_SIZE", gl.getMaxUniformBlockSize());
//		log_sandbox.info("{:46} [ {:9} ]", "GL_MAX_UNIFORM_BUFFER_BINDINGS", gl.getMaxUniformBufferBindings());
//		log_sandbox.info("{:46} [ {:9} ]", "GL_MAX_VERTEX_ATTRIBS", gl.getMaxVertexAttribs());
//		log_sandbox.info("{:46} [ {:9} ]", "GL_MAX_VERTEX_UNIFORM_COMPONENTS", gl.getMaxVertexUniformComponents());
//		log_sandbox.info("{:46} [ {:9} ]", "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS", gl.getMaxCombinedTextureImageUnits());
//		log_sandbox.info("{:46} [ {:9} ]", "GL_MAX_TEXTURE_SIZE", gl.getMaxTextureSize());

		// TODO P1: These should be queue functions, or not?, Maybe I do need to setup some defaults, and that is remote...
		//			But! I will need a nicer syntax, so make gl private
		remote.gl.capability.blend.enable();
		remote.gl.capability.cullFace.enable();
		remote.gl.capability.depthTest.enable();
		remote.gl.depthFunction.less();
		remote.gl.blendFunction(libv::gl::BlendFunction::SourceAlpha, libv::gl::BlendFunction::One_Minus_SourceAlpha);

		remote.gl.cullFace.back();
		remote.gl.frontFace.ccw();
		remote.gl.polygonMode(true ? libv::gl::Mode::Fill : libv::gl::Mode::Line);

		auto update = libv::glr::MeshUpdate<libv::glr::VertexPT>{
			libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

		update
			.vertex(libv::vec3f{-1.f, -1.f, +1.f}, libv::vec2f{0.f, 0.f})
			.vertex(libv::vec3f{+1.f, -1.f, +1.f}, libv::vec2f{1.f, 0.f})
			.vertex(libv::vec3f{+1.f, +1.f, +1.f}, libv::vec2f{1.f, 1.f})
			.vertex(libv::vec3f{-1.f, +1.f, +1.f}, libv::vec2f{0.f, 1.f})

			.vertex(libv::vec3f{+1.f, +1.f, -1.f}, libv::vec2f{0.f, 0.f})
			.vertex(libv::vec3f{+1.f, -1.f, -1.f}, libv::vec2f{1.f, 0.f})
			.vertex(libv::vec3f{-1.f, -1.f, -1.f}, libv::vec2f{1.f, 1.f})
			.vertex(libv::vec3f{-1.f, +1.f, -1.f}, libv::vec2f{0.f, 1.f})

			.vertex(libv::vec3f{+1.f, +1.f, +1.f}, libv::vec2f{1.f, 1.f})
			.vertex(libv::vec3f{+1.f, +1.f, -1.f}, libv::vec2f{1.f, 0.f})
			.vertex(libv::vec3f{-1.f, +1.f, -1.f}, libv::vec2f{0.f, 0.f})
			.vertex(libv::vec3f{-1.f, +1.f, +1.f}, libv::vec2f{0.f, 1.f})

			.vertex(libv::vec3f{-1.f, -1.f, -1.f}, libv::vec2f{1.f, 1.f})
			.vertex(libv::vec3f{+1.f, -1.f, -1.f}, libv::vec2f{1.f, 0.f})
			.vertex(libv::vec3f{+1.f, -1.f, +1.f}, libv::vec2f{0.f, 0.f})
			.vertex(libv::vec3f{-1.f, -1.f, +1.f}, libv::vec2f{0.f, 1.f})

			.vertex(libv::vec3f{+1.f, -1.f, -1.f}, libv::vec2f{0.f, 0.f})
			.vertex(libv::vec3f{+1.f, +1.f, -1.f}, libv::vec2f{1.f, 0.f})
			.vertex(libv::vec3f{+1.f, +1.f, +1.f}, libv::vec2f{1.f, 1.f})
			.vertex(libv::vec3f{+1.f, -1.f, +1.f}, libv::vec2f{0.f, 1.f})

			.vertex(libv::vec3f{-1.f, +1.f, +1.f}, libv::vec2f{0.f, 0.f})
			.vertex(libv::vec3f{-1.f, +1.f, -1.f}, libv::vec2f{1.f, 0.f})
			.vertex(libv::vec3f{-1.f, -1.f, -1.f}, libv::vec2f{1.f, 1.f})
			.vertex(libv::vec3f{-1.f, -1.f, +1.f}, libv::vec2f{0.f, 1.f})
		;

		update.index({
			 0,  1,  2,  0,  2,  3,
			 4,  5,  6,  4,  6,  7,
			 8,  9, 10,  8, 10, 11,
			12, 13, 14, 12, 14, 15,
			16, 17, 18, 16, 18, 19,
			20, 21, 22, 20, 22, 23});

		boxMesh.update(std::move(update));

		// TODO P2: These read_file_or_throw exceptions here will leak openGL resources! Same goes for the other gl sandboxes
		shaderTest1Fragment.createCompile(libv::gl::ShaderType::Fragment, libv::read_file_or_throw("res/shader/red.fs"));
		shaderTest1Vertex.createCompile(libv::gl::ShaderType::Vertex, libv::read_file_or_throw("res/shader/red.vs"));
		programTest1.createLink(shaderTest1Fragment, shaderTest1Vertex);
		programTest1.assign(uniformTest1MVPmat, "MVPmat");

		libv::gl::checkGL();
	}

	void render() {

		angle += 0.5f;

		remote.gl.projection = libv::perspective(1.f, 1.f * WINDOW_WIDTH / WINDOW_HEIGHT, 1.f, 1000.f);
		remote.gl.view = libv::lookAt(libv::vec3f(5.f, 3.f, 5.f), libv::vec3f(0.f, 0.f, 0.f), libv::vec3f(0.f, 1.f, 0.f));
		remote.gl.view.rotate(angle / 90.f, libv::vec3f(0.f, 1.f, 0.f));
		remote.gl.model = libv::identity<4, float>();

		libv::glr::Queue queue;

		{
			// const auto pStackGuard = queue.projection.push_guard();
			// const auto vStackGuard = queue.view.push_guard();
			// const auto mStackGuard = queue.model.push_guard();

			// const auto tx_guard = queue.texture_guard(libv::gl::TextureChannel::diffuse, uniformTest1TextureDiffuseSampler, texturePlane);
			// queue.uniform(uniformColor, color);

			// const auto pr_guard = queue.use(programTest1);

			queue.render(boxMesh);
		}

		remote.clearColor(0.236f, 0.311f, 0.311f, 1.f);
		remote.clearDepth();

		programTest1.use();
		uniformTest1MVPmat = remote.gl.mvp();
		remote.execute(std::move(queue));

		libv::gl::checkGL();
	}

	~Sandbox() {
		remote.destroy();
	}
};

// Runner ------------------------------------------------------------------------------------------

#define CHECK_GLEW_SUPPORT(ext) log_sandbox.info("{:46} [{}]", #ext, glewIsSupported(#ext) ? " SUPPORTED " : "UNSUPPORTED")

void initGLEW() {
	if (GLenum err = glewInit() != GLEW_OK)
		log_sandbox.error("Failed to initialize glew: {}", glewGetErrorString(err));

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
}

static void error_callback(int code, const char* description) {
	log_sandbox.error("GLFW {}: {}", code, description);
}

// -------------------------------------------------------------------------------------------------

auto running = std::atomic_bool{true};

int main(void) {
	std::cout << libv::logger;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		log_sandbox.fatal("Failed to initialize GLFW.");
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window;

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		log_sandbox.fatal("Failed to create GLFW window.");
		return EXIT_FAILURE;
	}
	glfwSetWindowPos(window, 200, 200);

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, [](GLFWwindow*, int key, int, int, int) {
		if (key == GLFW_KEY_ESCAPE)
			running = false;
	});
	glfwSwapInterval(1);

	initGLEW();

	libv::gl::checkGL();

	try {
		Sandbox sandbox;

		libv::Timer timer;
		size_t time = 0, i = 0;

		while (running && !glfwWindowShouldClose(window)) {
			libv::gl::checkGL();
			sandbox.render();

			glfwSwapBuffers(window);
			glfwPollEvents();

			i++;
			time += timer.time().count();
			if (time > 1000000000) {
				log_sandbox.info("FPS: {}", i);
				i = 0;
				time -= 1000000000;
			}
		}

		glfwMakeContextCurrent(nullptr);
		glfwDestroyWindow(window);
		glfwTerminate();

		return EXIT_SUCCESS;

		// TODO P2: merge this exception handling, return EXIT, log.fatal with the rest of the gl sandboxes. This might require merging the glr branches first!
	} catch (const std::system_error& e) {
		log_sandbox.fatal("Exception caught: {} - {}: {}", e.what(), e.code(), e.code().message());
		return EXIT_FAILURE;

	} catch (const std::exception& e) {
		log_sandbox.fatal("Exception caught: {}", e.what());
		return EXIT_FAILURE;
	}
}
