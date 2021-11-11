// Project: libv.gl, File: sandbox/libv_gl_runner.hpp

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

//	log_sandbox.debug("GL Vendor   {}", glGetString(GL_VENDOR));
//	log_sandbox.debug("GL Renderer {}", glGetString(GL_RENDERER));
//	log_sandbox.debug("GL Version  {}", glGetString(GL_VERSION));
//
//  const auto checkGLSupport = [](const char* ext) {
//      log_sandbox.debug("{:46} [{}]", ext, glewIsSupported(ext) ? " SUPPORTED " : "UNSUPPORTED");
//  };
//
//	checkGLSupport("GL_VERSION_3_3");
//	checkGLSupport("GL_VERSION_4_5");
//	checkGLSupport("GL_ARB_direct_state_access");
//	checkGLSupport("GL_ARB_draw_elements_base_vertex");
//	checkGLSupport("GL_ARB_gpu_shader_fp64");
//	checkGLSupport("GL_ARB_gpu_shader_int64");
//	checkGLSupport("GL_ARB_sampler_objects");
//	checkGLSupport("GL_ARB_vertex_attrib_64bit");
//	checkGLSupport("GL_ARB_vertex_attrib_binding");
//	checkGLSupport("GL_EXT_texture_compression_s3tc");

auto running = std::atomic_bool{true};

/// Expected signature:
///	struct Sandbox {
///		Sandbox();
///		void update(const std::chrono::duration deltaTime);
///		void render();
///	};
template <typename Sandbox>
int run_sandbox(const std::string& title, const uint32_t window_height, const uint32_t window_width, int samples = 4, int depth_bits = 32) {
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
	glfwWindowHint(GLFW_SAMPLES, samples);

//	glfwWindowHint(GLFW_SRGB_CAPABLE, false);
//	glfwWindowHint(GLFW_SRGB_CAPABLE, true);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 0);
	glfwWindowHint(GLFW_DEPTH_BITS, depth_bits);
	glfwWindowHint(GLFW_STENCIL_BITS, 0);

	// --- Windowed ------------------------------------------------------------------------------------

	GLFWwindow* window = glfwCreateWindow(window_width, window_height, title.c_str(), nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		log_sandbox.fatal("Failed to create GLFW window.");
		return EXIT_FAILURE;
	}
	glfwSetWindowPos(window, 200, 200);

	// --- Windowed-Fullscreen -------------------------------------------------------------------------

//	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
//	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
//
//	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
//	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
//	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
//	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
//	glfwWindowHint(GLFW_AUTO_ICONIFY, false);
//
//	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, title.c_str(), monitor, nullptr);

	// --- Fullscreen ----------------------------------------------------------------------------------

//	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
////	glfwWindowHint(GLFW_AUTO_ICONIFY, true);
//	GLFWwindow* window = glfwCreateWindow(window_width, window_height, title.c_str(), monitor, nullptr);
//	if (!window) {
//		glfwTerminate();
//		log_sandbox.fatal("Failed to create GLFW window.");
//		return EXIT_FAILURE;
//	}

	// --- Windowed-Borderless -------------------------------------------------------------------------

//	glfwWindowHint(GLFW_DECORATED, false);
//	GLFWwindow* window = glfwCreateWindow(window_width, window_height, title.c_str(), nullptr, nullptr);
//	if (!window) {
//		glfwTerminate();
//		log_sandbox.fatal("Failed to create GLFW window.");
//		return EXIT_FAILURE;
//	}

	// --- Windowed-Borderless2 ------------------------------------------------------------------------

//	glfwWindowHint(GLFW_DECORATED, false);
//	glfwWindowHint(GLFW_MAXIMIZED, true);
//	GLFWwindow* window = glfwCreateWindow(window_width, window_height, title.c_str(), nullptr, nullptr);
//	if (!window) {
//		glfwTerminate();
//		log_sandbox.fatal("Failed to create GLFW window.");
//		return EXIT_FAILURE;
//	}

	// --- Windowed-Borderless3 ------------------------------------------------------------------------

//	glfwWindowHint(GLFW_DECORATED, false);
//	glfwWindowHint(GLFW_VISIBLE, false);
//	int ignore;
//	GLFWmonitor* monitor = glfwGetMonitors(&ignore)[0];
////	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
//	int x, y, w, h;
//	glfwGetMonitorWorkarea(monitor, &x, &y, &w, &h);
//	GLFWwindow* window = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);
//	if (!window) {
//		glfwTerminate();
//		log_sandbox.fatal("Failed to create GLFW window.");
//		return EXIT_FAILURE;
//	}
//	glfwSetWindowPos(window, x, y);
//	glfwShowWindow(window);

	// -------------------------------------------------------------------------------------------------

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, [](GLFWwindow* window_, int key, int scancode, int action, int mods) {
		auto& sandbox = *reinterpret_cast<Sandbox*>(glfwGetWindowUserPointer(window_));

		if constexpr (requires { sandbox.onKey(key, scancode, action, mods); })
			sandbox.onKey(key, scancode, action, mods);

		if (key == GLFW_KEY_ESCAPE)
			running = false;
	});
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window_, int width, int height) {
		auto& sandbox = *reinterpret_cast<Sandbox*>(glfwGetWindowUserPointer(window_));

		if constexpr (requires { sandbox.onResize(width, height); })
			sandbox.onResize(width, height);
	});
	glfwSwapInterval(1);

	try {
		Sandbox sandbox;
		glfwSetWindowUserPointer(window, &sandbox);

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

				log_sandbox.info("--- Report of {} frame ----------------------------------------------", time_outside.count());
				log_sandbox.info("    | Poll μs  |Update μs |Render μs | Other μs | Swap μs  |  Sum μs  ");
				log_sandbox.info("MIN |{:10.3f}|{:10.3f}|{:10.3f}|{:10.3f}|{:10.3f}|{:10.3f}",
						static_cast<float>(time_poll.min().count()) / 1000.f,
						static_cast<float>(time_update.min().count()) / 1000.f,
						static_cast<float>(time_render.min().count()) / 1000.f,
						static_cast<float>(time_outside.min().count()) / 1000.f,
						static_cast<float>(time_swap.min().count()) / 1000.f,
						static_cast<float>((time_outside.min() + time_render.min() + time_render.min() + time_swap.min() + time_poll.min()).count()) / 1000.f
				);

				log_sandbox.info("AVG |{:10.3f}|{:10.3f}|{:10.3f}|{:10.3f}|{:10.3f}|{:10.3f}",
						static_cast<float>(time_poll.avg().count()) / 1000.f,
						static_cast<float>(time_update.avg().count()) / 1000.f,
						static_cast<float>(time_render.avg().count()) / 1000.f,
						static_cast<float>(time_outside.avg().count()) / 1000.f,
						static_cast<float>(time_swap.avg().count()) / 1000.f,
						static_cast<float>((time_outside.avg() + time_render.avg() + time_render.avg() + time_swap.avg() + time_poll.avg()).count()) / 1000.f
				);

				log_sandbox.info("MAX |{:10.3f}|{:10.3f}|{:10.3f}|{:10.3f}|{:10.3f}|{:10.3f}",
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
