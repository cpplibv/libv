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

bool enable_vsync = true;

// Runner ------------------------------------------------------------------------------------------

void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
		const GLchar* message, const void* userData) {

	(void) length;
	(void) userData;

	const auto source_str = [&] {
		switch (source) {
		case GL_DEBUG_SOURCE_API:             return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "Window System";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
		case GL_DEBUG_SOURCE_THIRD_PARTY:     return "Third Party";
		case GL_DEBUG_SOURCE_APPLICATION:     return "Application";
		case GL_DEBUG_SOURCE_OTHER:           return "Other";
		default:                              return "?";
		}
	}();

	const auto type_str = [&] {
		switch (type) {
		case GL_DEBUG_TYPE_ERROR:               return "Error";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behaviour";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "Undefined Behaviour";
		case GL_DEBUG_TYPE_PORTABILITY:         return "Portability";
		case GL_DEBUG_TYPE_PERFORMANCE:         return "Performance";
		case GL_DEBUG_TYPE_MARKER:              return "Marker";
		case GL_DEBUG_TYPE_PUSH_GROUP:          return "Push Group";
		case GL_DEBUG_TYPE_POP_GROUP:           return "Pop Group";
		case GL_DEBUG_TYPE_OTHER:               return "Other";
		default:                                return "?";
		}
	}();

	const auto severity_str = [&] {
		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:         return "high";
		case GL_DEBUG_SEVERITY_MEDIUM:       return "medium";
		case GL_DEBUG_SEVERITY_LOW:          return "low";
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "notification";
		default:                             return "?";
		}
	}();


	if (type == GL_DEBUG_TYPE_ERROR)
		log_sandbox.error("OpenGL: src {} ({}), type {} ({:X}), id {}, severity {} ({:X}), message: {}",
				source_str, source, type_str, type, id, severity_str, severity, message);

	const auto suppress_unimportant_messages = true;
	if (suppress_unimportant_messages) {
		if (id == 131218)
			// Program/shader state performance warning: Vertex shader in program <X> is being recompiled based on GL state.
			return;

		if (id == 131185)
			// Buffer detailed info: Buffer object <X> (...) will use VIDEO memory as the source for buffer object operations.
			// Buffer detailed info: Buffer object <X> (...) has been mapped in HOST memory.
			// Buffer detailed info: Buffer object <X> (...) stored in VIDEO memory has been updated.
			return;
	}

	log_sandbox.trace("OpenGL: src {} ({}), type {} ({:X}), id {}, severity {} ({:X}), message: {}",
			source_str, source, type_str, type, id, severity_str, severity, message);
};

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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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

	// -------------------------------------------------------------------------------------------------

	if (GLenum err = glewInit() != GLEW_OK) {
		log_sandbox.error("Failed to initialize glew: {} (Does the current thread has an OpenGL context?)", glewGetErrorString(err));
		return EXIT_FAILURE;
	}

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(debugCallback, nullptr);

	// -------------------------------------------------------------------------------------------------

	glfwSetKeyCallback(window, [](GLFWwindow* window_, int key, int scancode, int action, int mods) {
		auto& sandbox = *reinterpret_cast<Sandbox*>(glfwGetWindowUserPointer(window_));

		if constexpr (requires { sandbox.onKey(key, scancode, action, mods); })
			sandbox.onKey(key, scancode, action, mods);

		if (key == GLFW_KEY_ESCAPE)
			running = false;

		if (action == GLFW_RELEASE)
			return;

		if (key == GLFW_KEY_F2) {
			enable_vsync = !enable_vsync;
			glfwSwapInterval(enable_vsync ? 1 : 0);
		}
	});
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window_, int width, int height) {
		auto& sandbox = *reinterpret_cast<Sandbox*>(glfwGetWindowUserPointer(window_));

		if constexpr (requires { sandbox.onResize(width, height); })
			sandbox.onResize(width, height);
	});
	glfwSwapInterval(enable_vsync ? 1 : 0);

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

				log_sandbox.info("--- Report of {} frame ---------------------------------------------------------", time_outside.count());
				log_sandbox.info("    | Poll μs  |Update μs |Render μs | Other μs | Swap μs  |  Sum μs  |NotSwap μs");
				log_sandbox.info("MIN |{:10.3f}|{:10.3f}|{:10.3f}|{:10.3f}|{:10.3f}|{:10.3f}|{:10.3f}",
						static_cast<float>(time_poll.min().count()) / 1000.f,
						static_cast<float>(time_update.min().count()) / 1000.f,
						static_cast<float>(time_render.min().count()) / 1000.f,
						static_cast<float>(time_outside.min().count()) / 1000.f,
						static_cast<float>(time_swap.min().count()) / 1000.f,
						static_cast<float>((time_outside.min() + time_render.min() + time_render.min() + time_swap.min() + time_poll.min()).count()) / 1000.f,
						static_cast<float>((time_outside.min() + time_render.min() + time_render.min() + time_swap.min() + time_poll.min()).count()) / 1000.f - static_cast<float>(time_swap.min().count()) / 1000.f
				);

				log_sandbox.info("AVG |{:10.3f}|{:10.3f}|{:10.3f}|{:10.3f}|{:10.3f}|{:10.3f}|{:10.3f}",
						static_cast<float>(time_poll.avg().count()) / 1000.f,
						static_cast<float>(time_update.avg().count()) / 1000.f,
						static_cast<float>(time_render.avg().count()) / 1000.f,
						static_cast<float>(time_outside.avg().count()) / 1000.f,
						static_cast<float>(time_swap.avg().count()) / 1000.f,
						static_cast<float>((time_outside.avg() + time_render.avg() + time_render.avg() + time_swap.avg() + time_poll.avg()).count()) / 1000.f,
						static_cast<float>((time_outside.avg() + time_render.avg() + time_render.avg() + time_swap.avg() + time_poll.avg()).count()) / 1000.f - static_cast<float>(time_swap.avg().count()) / 1000.f
				);

				log_sandbox.info("MAX |{:10.3f}|{:10.3f}|{:10.3f}|{:10.3f}|{:10.3f}|{:10.3f}|{:10.3f}",
						static_cast<float>(time_poll.max().count()) / 1000.f,
						static_cast<float>(time_update.max().count()) / 1000.f,
						static_cast<float>(time_render.max().count()) / 1000.f,
						static_cast<float>(time_outside.max().count()) / 1000.f,
						static_cast<float>(time_swap.max().count()) / 1000.f,
						static_cast<float>((time_outside.max() + time_render.max() + time_render.max() + time_swap.max() + time_poll.max()).count()) / 1000.f,
						static_cast<float>((time_outside.max() + time_render.max() + time_render.max() + time_swap.max() + time_poll.max()).count()) / 1000.f - static_cast<float>(time_swap.max().count()) / 1000.f
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
