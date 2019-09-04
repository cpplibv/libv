// File: libv_gl_main3.cpp, Created on 2017.11.17. at 22:38, Author: Vader

// ext
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// libv
#include <libv/log/log.hpp>
#include <libv/utility/stat_counter.hpp>
#include <libv/utility/timer.hpp>
// std
#include <atomic>
#include <chrono>
#include <string>
// pro
#include <libv/gl/check.hpp>


// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_sandbox{libv::logger_stream, "sandbox"};

// Runner ------------------------------------------------------------------------------------------

void checkGLSupport(const char* ext) {
	log_sandbox.debug("{:46} [{}]", ext, glewIsSupported(ext) ? " SUPPORTED " : "UNSUPPORTED");
}

void initGLEW() {
	glewExperimental = true;
	if (GLenum err = glewInit() != GLEW_OK)
		log_sandbox.error("Failed to initialize glew: {}", glewGetErrorString(err));

	log_sandbox.debug("GL Vendor   {}", glGetString(GL_VENDOR));
	log_sandbox.debug("GL Renderer {}", glGetString(GL_RENDERER));
	log_sandbox.debug("GL Version  {}", glGetString(GL_VERSION));

	checkGLSupport("GL_VERSION_3_3");
	checkGLSupport("GL_VERSION_4_5");
	checkGLSupport("GL_ARB_direct_state_access");
	checkGLSupport("GL_ARB_draw_elements_base_vertex");
	checkGLSupport("GL_ARB_gpu_shader_fp64");
	checkGLSupport("GL_ARB_gpu_shader_int64");
	checkGLSupport("GL_ARB_sampler_objects");
	checkGLSupport("GL_ARB_vertex_attrib_64bit");
	checkGLSupport("GL_ARB_vertex_attrib_binding");
	checkGLSupport("GL_EXT_texture_compression_s3tc");

	libv::gl::checkGL();
}

auto running = std::atomic_bool{true};

/// Expected signature:
///	struct Sandbox {
///		Sandbox();
///		void update(const std::chrono::duration deltaTime);
///		void render();
///	};
template <typename Sandbox>
int run_sandbox(const std::string& title, const uint32_t window_height, const uint32_t window_width) {
	glfwSetErrorCallback([](int code, const char* description) {
		log_sandbox.error("GLFW {}: {}", code, description);
	});

	if (!glfwInit()) {
		log_sandbox.fatal("Failed to initialize GLFW.");
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window;

	window = glfwCreateWindow(window_width, window_height, title.c_str(), nullptr, nullptr);
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

	// TODO P1: Make glew init part of libv.gl
	initGLEW();

	libv::gl::checkGL();

	try {
		Sandbox sandbox;

		libv::stat_counter<std::chrono::nanoseconds> time_outside;
		libv::stat_counter<std::chrono::nanoseconds> time_update;
		libv::stat_counter<std::chrono::nanoseconds> time_render;
		libv::stat_counter<std::chrono::nanoseconds> time_swap;
		libv::stat_counter<std::chrono::nanoseconds> time_poll;

		libv::Timer timer;
		libv::Timer timer_update;
		libv::Timer timer_print;
		std::chrono::seconds print_interval{10};

		while (running && !glfwWindowShouldClose(window)) {
			libv::gl::checkGL();
			time_outside.record(timer.time());

			sandbox.update(timer_update.time());
			time_update.record(timer.time());

			sandbox.render();
			time_render.record(timer.time());

			glfwSwapBuffers(window);
			time_swap.record(timer.time());

			glfwPollEvents();
			time_poll.record(timer.time());

			if (timer_print.elapsed() > print_interval) {
				timer_print.adjust(print_interval);

				log_sandbox.info("--- Report of {} frame --------------------------------------", time_outside.count());
				log_sandbox.info("MIN) Poll: {:7.3f}μs, Update: {:7.3f}μs, Render: {:7.3f}μs, Other: {:7.3f}μs, Swap: {:7.3f}μs, Sum: {:7.3f}μs",
						static_cast<float>(time_poll.min().count()) / 1000.f,
						static_cast<float>(time_update.min().count()) / 1000.f,
						static_cast<float>(time_render.min().count()) / 1000.f,
						static_cast<float>(time_outside.min().count()) / 1000.f,
						static_cast<float>(time_swap.min().count()) / 1000.f,
						static_cast<float>((time_outside.min() + time_render.min() + time_render.min() + time_swap.min() + time_poll.min()).count()) / 1000.f
				);

				log_sandbox.info("AVG) Poll: {:7.3f}μs, Update: {:7.3f}μs, Render: {:7.3f}μs, Other: {:7.3f}μs, Swap: {:7.3f}μs, Sum: {:7.3f}μs",
						static_cast<float>(time_poll.avg().count()) / 1000.f,
						static_cast<float>(time_update.avg().count()) / 1000.f,
						static_cast<float>(time_render.avg().count()) / 1000.f,
						static_cast<float>(time_outside.avg().count()) / 1000.f,
						static_cast<float>(time_swap.avg().count()) / 1000.f,
						static_cast<float>((time_outside.avg() + time_render.avg() + time_render.avg() + time_swap.avg() + time_poll.avg()).count()) / 1000.f
				);

				log_sandbox.info("MAX) Poll: {:7.3f}μs, Update: {:7.3f}μs, Render: {:7.3f}μs, Other: {:7.3f}μs, Swap: {:7.3f}μs, Sum: {:7.3f}μs",
						static_cast<float>(time_poll.max().count()) / 1000.f,
						static_cast<float>(time_update.max().count()) / 1000.f,
						static_cast<float>(time_render.max().count()) / 1000.f,
						static_cast<float>(time_outside.max().count()) / 1000.f,
						static_cast<float>(time_swap.max().count()) / 1000.f,
						static_cast<float>((time_outside.max() + time_render.max() + time_render.max() + time_swap.max() + time_poll.max()).count()) / 1000.f
				);

				time_outside.reset();
				time_update.reset();
				time_render.reset();
				time_swap.reset();
				time_poll.reset();
			}
		}

	} catch (const std::system_error& e) {
		log_sandbox.fatal("Exception caught: {} - {}: {}", e.what(), e.code(), e.code().message());
		return EXIT_FAILURE;

	} catch (const std::exception& e) {
		log_sandbox.fatal("Exception caught: {}", e.what());
		return EXIT_FAILURE;
	}

	libv::gl::checkGL();

	glfwMakeContextCurrent(nullptr);
	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}
