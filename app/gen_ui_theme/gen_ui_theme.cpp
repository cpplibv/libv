// Project: libv, File: app/gen_ui_theme/gen_ui_theme.cpp, Author: Császár Mátyás [Vader]

// ext
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// libv
#include <libv/fsw/watcher.hpp>
#include <libv/gl/check.hpp>
#include <libv/gl/enum.hpp>
#include <libv/glr/attribute.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/program.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/remote.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_block_binding.hpp>
#include <libv/glr/uniform_buffer.hpp>
#include <libv/log/log.hpp>
#include <libv/lua/lua.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/timer.hpp>
// std
#include <iostream>
#include <atomic>
#include <chrono>
#include <mutex>
#include <string>


// -------------------------------------------------------------------------------------------------

constexpr uint32_t WINDOW_WIDTH = 1152 + 30;
constexpr uint32_t WINDOW_HEIGHT = 1024 + 20;

// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_sandbox{libv::logger_stream, "gen_ui_theme"};

// Runner ------------------------------------------------------------------------------------------

auto running = std::atomic_bool{true};
auto pulse = std::atomic_bool{true};

void initGLEW() {
	glewExperimental = true;
	if (GLenum err = glewInit() != GLEW_OK)
		log_sandbox.error("Failed to initialize glew: {}", glewGetErrorString(err));

	libv::gl::checkGL();
}

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

	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 0);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);
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

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, [](GLFWwindow*, int key, int, int action, int) {
		if (action != GLFW_RELEASE)
			return;

		if (key == GLFW_KEY_ESCAPE)
			running = false;
		if (key == GLFW_KEY_SPACE)
			pulse = !pulse;
	});
	glfwSwapInterval(1);

	initGLEW();

	libv::gl::checkGL();

	try {
		Sandbox sandbox;

		libv::Timer timer_update;

		while (running && !glfwWindowShouldClose(window)) {
			libv::gl::checkGL();
			sandbox.update(timer_update.time());
			sandbox.render();
			glfwSwapBuffers(window);
			glfwPollEvents();
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

// =================================================================================================

const auto shader_texture_vs = R"(
#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 8) in vec2 vertexTexture0;

out vec2 fragmentTexture0;

layout(std140) uniform Sphere {
	mat4 matMVP;
	mat4 matM;
	vec3 color;
};

void main() {
	gl_Position = matMVP * vec4(vertexPosition, 1);
	fragmentTexture0 = vertexTexture0;
}
)";

const auto shader_texture_fs = R"(
#version 330 core

in vec3 fragmentNormal;
in vec2 fragmentTexture0;

out vec4 output;

uniform sampler2D texture0Sampler;

layout(std140) uniform Sphere {
	mat4 matMVP;
	mat4 matM;
	vec3 color;
};

void main() {
	output = texture(texture0Sampler, fragmentTexture0).rgba;
}
)";

constexpr auto attribute_position  = libv::glr::Attribute<0, libv::vec3f>{};
constexpr auto attribute_texture0  = libv::glr::Attribute<8, libv::vec2f>{};

const auto uniformBlock_sphere   = libv::glr::UniformBlockBinding{0, "Sphere"};

constexpr auto textureChannel_diffuse = libv::gl::TextureChannel{0};

// -------------------------------------------------------------------------------------------------

struct SphereUniformLayout {
	libv::glr::Uniform_mat4f matMVP;
	libv::glr::Uniform_mat4f matM;
	libv::glr::Uniform_vec3f color;

	LIBV_REFLECTION_ACCESS(matMVP);
	LIBV_REFLECTION_ACCESS(matM);
	LIBV_REFLECTION_ACCESS(color);
};
const auto sphere_layout = libv::glr::layout_std140<SphereUniformLayout>(uniformBlock_sphere);

// -------------------------------------------------------------------------------------------------

struct Sandbox {
	libv::vec2i windowSize = {WINDOW_WIDTH, WINDOW_HEIGHT};
	libv::vec2i textureSize = {128, 128};

	libv::glr::Remote remote; // Remote has to be the first data member to cleanup gl resources

	libv::glr::Program sphere_program;
	libv::glr::Mesh sphere_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	libv::glr::UniformBuffer sphere_uniforms{libv::gl::BufferUsage::StreamDraw};
	libv::glr::Uniform_vec3f sphere_uniform_shift;
	libv::glr::Uniform_float sphere_uniform_time;
	libv::glr::Uniform_texture sphere_uniform_texture0;
	libv::glr::Uniform_texture sphere_uniform_texture1;
	libv::glr::Texture2D::R8_G8_B8_A8 sphere_texture0;

	libv::fsw::FileWatcher file_watcher;
	std::string script_file = "app/gen_ui_theme/theme_slate.lua";
	std::vector<libv::vec4uc> texture_data;
	std::mutex texture_m;
	bool dirty = false;

	libv::lua::State lua = libv::lua::create_state(libv::lua::lualib::base | libv::lua::lualib::vec);

	std::chrono::duration<float> time;

	// -------------------------------------------------------------------------------------------------

	Sandbox() {
		// --- Sphere ---
		sphere_program.vertex(shader_texture_vs);
		sphere_program.fragment(shader_texture_fs);
		sphere_program.block_binding(uniformBlock_sphere);
		sphere_program.assign(sphere_uniform_texture0, "texture0Sampler", textureChannel_diffuse);

		sphere_texture0.storage(1, textureSize);
		sphere_texture0.set(libv::gl::MagFilter::Nearest);

		{
			auto position = sphere_mesh.attribute(attribute_position);
			auto texture0 = sphere_mesh.attribute(attribute_texture0);
			auto index = sphere_mesh.index();

			const auto add_lod = [&](int i, auto pos, auto size) {
				position(pos.x +      0, pos.y +      0, 0);
				position(pos.x + size.x, pos.y +      0, 0);
				position(pos.x + size.x, pos.y + size.y, 0);
				position(pos.x +      0, pos.y + size.y, 0);

				texture0(0, 0);
				texture0(1, 0);
				texture0(1, 1);
				texture0(0, 1);

				index.quad(i * 4 + 0, i * 4 + 1, i * 4 + 2, i * 4 + 3);
			};

			const auto ws = windowSize.cast<float>();
			const auto ts = textureSize.cast<float>();

			add_lod(0, libv::vec2f(0 + 10, (ws.y - ts.y) * 0.5f), ts);
			add_lod(1, libv::vec2f(ts.x + 20, 10), ts * 8.0f);
		}

		// --- Start ---

		remote.create();
		remote.enableDebug();

		// --- Start ---

		load_texture();
		file_watcher.subscribe_file(script_file, [this](const auto&) {
			load_texture();
		});
	}

	~Sandbox() {
	}

	void load_texture() {
		log_sandbox.info("Updating texture...");

		const auto guard = std::scoped_lock(texture_m);

		libv::Timer script_timer;

		lua["tex_size_x"] = textureSize.x;
		lua["tex_size_y"] = textureSize.y;
		auto result = lua.safe_script(libv::read_file_or_throw(script_file), sol::script_pass_on_error);

		if (!result.valid()) {
			sol::error err = result;
			return log_sandbox.error("Script execution failed: {}", err.what());
		}

		if (result.get_type() != sol::type::table) {
			return log_sandbox.error("Script did not return a table: {} - {}", libv::to_value(result.get_type()), std::string(result));
		}

		log_sandbox.info("Script execution successful: {:7.3f}ms", script_timer.timef_ms().count());

		sol::table result_table = result;

		texture_data.clear();
		texture_data.resize(textureSize.x * textureSize.y);
		int i = 0;
		for (const auto v : result_table) {
			if (!v.second.is<libv::vec4f>()) {
				log_sandbox.error("Element {} is not a vec4f", i);
				i++;
				continue;
			}

			libv::vec4f c = v.second.as<libv::vec4f>();
			texture_data[i].x = static_cast<uint8_t>(std::clamp(c.x, 0.0f, 1.0f) * 255.f);
			texture_data[i].y = static_cast<uint8_t>(std::clamp(c.y, 0.0f, 1.0f) * 255.f);
			texture_data[i].z = static_cast<uint8_t>(std::clamp(c.z, 0.0f, 1.0f) * 255.f);
			texture_data[i].w = static_cast<uint8_t>(std::clamp(c.w, 0.0f, 1.0f) * 255.f);
			i++;
		}

		dirty = true;
	}

	void update(const std::chrono::duration<float> deltaTime) {
		time += deltaTime;

		const auto guard = std::scoped_lock(this->texture_m);

		if (!dirty)
			return;

		sphere_texture0.image(0, {0, 0}, textureSize, texture_data.data());
	}

	void render() {
		auto queue = remote.queue();

		{
			auto& gl = queue;

			const auto guard_s = gl.state.push_guard();
			const auto guard_m = gl.model.push_guard();
			const auto guard_v = gl.view.push_guard();
			const auto guard_p = gl.projection.push_guard();

			gl.state.enableBlend();
			gl.state.blendSrc_SourceAlpha();
			gl.state.blendDst_One_Minus_SourceAlpha();

			gl.state.enableCullFace();
			gl.state.frontFaceCCW();
			gl.state.cullBackFace();

			gl.state.polygonModeFill();

			gl.state.enableDepthTest();
			gl.state.depthFunctionLess();

			const auto clear_v = !pulse ? 0.5f : std::sin(time.count()) * 0.5f + 0.5f;
			gl.setClearColor(clear_v, clear_v, clear_v, 1.0f);
			gl.clearColor();
			gl.clearDepth();

			gl.projection = libv::mat4f::ortho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
			gl.view = libv::mat4f::identity();
			gl.model = libv::mat4f::identity();

			{
				auto uniforms = sphere_uniforms.block_unique(sphere_layout);
				uniforms[sphere_layout.matMVP] = gl.mvp();
				uniforms[sphere_layout.matM] = gl.model;
				uniforms[sphere_layout.color] = libv::vec3f(1.0f, 1.0f, 1.0f);

				gl.program(sphere_program);
				gl.uniform(std::move(uniforms));
				gl.texture(sphere_texture0, textureChannel_diffuse);
				gl.render(sphere_mesh);
			}
		}

		remote.queue(std::move(queue));
		remote.execute();
	}
};

// Runner ------------------------------------------------------------------------------------------

int main() {
	std::cout << libv::logger_stream;
	return run_sandbox<Sandbox>("Gen UI Theme", WINDOW_HEIGHT, WINDOW_WIDTH);
}
