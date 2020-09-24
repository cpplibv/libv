// Project: libv.ui, File: app/gen_ui_theme/gen_ui_theme.cpp, Author: Császár Mátyás [Vader]

// ext
#include <GL/glew.h>
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



#include <libv/frame/frame.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/remote.hpp>


#include <libv/ui/component/button.hpp>
//#include <libv/ui/component/image.hpp>
#include <libv/ui/component/input_field.hpp>
//#include <libv/ui/component/label.hpp>
//#include <libv/ui/component/label_image.hpp>
//#include <libv/ui/component/panel_float.hpp>
//#include <libv/ui/component/panel_full.hpp>
//#include <libv/ui/component/panel_grid.hpp>
#include <libv/ui/component/panel_line.hpp>
//#include <libv/ui/component/quad.hpp>
//#include <libv/ui/component/scroll_bar.hpp>
//#include <libv/ui/component/scroll_pane.hpp>
//#include <libv/ui/component/stretch.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/ui/ui.hpp>

// -------------------------------------------------------------------------------------------------

constexpr uint32_t WINDOW_WIDTH = 1152 + 30;
constexpr uint32_t WINDOW_HEIGHT = 1024 + 20;

// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_sandbox{libv::logger_stream, "gen_ui_theme"};
inline std::atomic_bool pulse{true};

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
////class Sandbox : public libv::ui::Canvas<Sandbox> {
//	libv::vec2i windowSize = {WINDOW_WIDTH, WINDOW_HEIGHT};
//	libv::vec2i textureSize = {128, 128};
//
////	libv::glr::Remote remote; // Remote has to be the first data member to cleanup gl resources
//
//	libv::glr::Program sphere_program;
//	libv::glr::Mesh sphere_mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
//	libv::glr::UniformBuffer sphere_uniforms{libv::gl::BufferUsage::StreamDraw};
//	libv::glr::Uniform_vec3f sphere_uniform_shift;
//	libv::glr::Uniform_float sphere_uniform_time;
//	libv::glr::Uniform_texture sphere_uniform_texture0;
//	libv::glr::Uniform_texture sphere_uniform_texture1;
//	libv::glr::Texture2D::R8_G8_B8_A8 sphere_texture0;
//
//	libv::fsw::FileWatcher file_watcher;
//	std::string script_file = "app/gen_ui_theme/theme_slate.lua";
//	std::vector<libv::vec4uc> texture_data;
//	std::mutex texture_m;
//	bool dirty = false;
//
//	libv::lua::State lua = libv::lua::create_state(libv::lua::lualib::base | libv::lua::lualib::vec);
//
//	std::chrono::duration<float> time;
//
//	// -------------------------------------------------------------------------------------------------
//
//	Sandbox() {
//		// --- Sphere ---
//		sphere_program.vertex(shader_texture_vs);
//		sphere_program.fragment(shader_texture_fs);
//		sphere_program.block_binding(uniformBlock_sphere);
//		sphere_program.assign(sphere_uniform_texture0, "texture0Sampler", textureChannel_diffuse);
//
//		sphere_texture0.storage(1, textureSize);
//		sphere_texture0.set(libv::gl::MagFilter::Nearest);
//
//		{
//			auto position = sphere_mesh.attribute(attribute_position);
//			auto texture0 = sphere_mesh.attribute(attribute_texture0);
//			auto index = sphere_mesh.index();
//
//			const auto add_lod = [&](int i, auto pos, auto size) {
//				position(pos.x +      0, pos.y +      0, 0);
//				position(pos.x + size.x, pos.y +      0, 0);
//				position(pos.x + size.x, pos.y + size.y, 0);
//				position(pos.x +      0, pos.y + size.y, 0);
//
//				texture0(0, 0);
//				texture0(1, 0);
//				texture0(1, 1);
//				texture0(0, 1);
//
//				index.quad(i * 4 + 0, i * 4 + 1, i * 4 + 2, i * 4 + 3);
//			};
//
//			const auto ws = windowSize.cast<float>();
//			const auto ts = textureSize.cast<float>();
//
//			add_lod(0, libv::vec2f(0 + 10, (ws.y - ts.y) * 0.5f), ts);
//			add_lod(1, libv::vec2f(ts.x + 20, 10), ts * 8.0f);
//		}
//
//		// --- Start ---
//
//		load_texture();
//		file_watcher.subscribe_file(script_file, [this](const auto&) {
//			load_texture();
//		});
//	}
//
//	~Sandbox() {
//	}
//
//	void load_texture() {
//		log_sandbox.info("Updating texture...");
//
//		const auto guard = std::scoped_lock(texture_m);
//
//		libv::Timer script_timer;
//
//		lua["tex_size_x"] = textureSize.x;
//		lua["tex_size_y"] = textureSize.y;
//		auto result = lua.safe_script(libv::read_file_or_throw(script_file), sol::script_pass_on_error);
//
//		if (!result.valid()) {
//			sol::error err = result;
//			return log_sandbox.error("Script execution failed: {}", err.what());
//		}
//
//		if (result.get_type() != sol::type::table) {
//			return log_sandbox.error("Script did not return a table: {} - {}", libv::to_value(result.get_type()), std::string(result));
//		}
//
//		log_sandbox.info("Script execution successful: {:7.3f}ms", script_timer.timef_ms().count());
//
//		sol::table result_table = result;
//
//		texture_data.clear();
//		texture_data.resize(textureSize.x * textureSize.y);
//		int i = 0;
//		for (const auto v : result_table) {
//			if (!v.second.is<libv::vec4f>()) {
//				log_sandbox.error("Element {} is not a vec4f", i);
//				i++;
//				continue;
//			}
//
//			libv::vec4f c = v.second.as<libv::vec4f>();
////			texture_data[i] = (libv::vec::clamp(c, 0.0f, 1.0f) * 255.f).cast<uint8_t>();
//			texture_data[i].x = static_cast<uint8_t>(std::clamp(c.x, 0.0f, 1.0f) * 255.f);
//			texture_data[i].y = static_cast<uint8_t>(std::clamp(c.y, 0.0f, 1.0f) * 255.f);
//			texture_data[i].z = static_cast<uint8_t>(std::clamp(c.z, 0.0f, 1.0f) * 255.f);
//			texture_data[i].w = static_cast<uint8_t>(std::clamp(c.w, 0.0f, 1.0f) * 255.f);
//			i++;
//		}
//
//		dirty = true;
//	}
//
//	void update(const std::chrono::duration<float> deltaTime) {
//		time += deltaTime;
//
//		const auto guard = std::scoped_lock(this->texture_m);
//
//		if (!dirty)
//			return;
//
//		sphere_texture0.image(0, {0, 0}, textureSize, texture_data.data());
//	}
//
//	void render(libv::glr::Queue& gl) {
//		const auto guard_s = gl.state.push_guard();
//		const auto guard_m = gl.model.push_guard();
//		const auto guard_v = gl.view.push_guard();
//		const auto guard_p = gl.projection.push_guard();
//
//		gl.state.enableBlend();
//		gl.state.blendSrc_SourceAlpha();
//		gl.state.blendDst_One_Minus_SourceAlpha();
//
//		gl.state.enableCullFace();
//		gl.state.frontFaceCCW();
//		gl.state.cullBackFace();
//
//		gl.state.polygonModeFill();
//
//		gl.state.enableDepthTest();
//		gl.state.depthFunctionLess();
//
//		const auto clear_v = !pulse ? 0.5f : std::sin(time.count()) * 0.5f + 0.5f;
//		gl.setClearColor(clear_v, clear_v, clear_v, 1.0f);
//		gl.clearColor();
//		gl.clearDepth();
//
//		gl.projection = libv::mat4f::ortho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
//		gl.view = libv::mat4f::identity();
//		gl.model = libv::mat4f::identity();
//
//		{
//			auto uniforms = sphere_uniforms.block_unique(sphere_layout);
//			uniforms[sphere_layout.matMVP] = gl.mvp();
//			uniforms[sphere_layout.matM] = gl.model;
//			uniforms[sphere_layout.color] = libv::vec3f(1.0f, 1.0f, 1.0f);
//
//			gl.program(sphere_program);
//			gl.uniform(std::move(uniforms));
//			gl.texture(sphere_texture0, textureChannel_diffuse);
//			gl.render(sphere_mesh);
//		}
//	}
};

// =================================================================================================

class GenUIThemeFrame : public libv::Frame {
private:
	Sandbox sandbox;
	libv::Timer update_timer;
//	libv::glr::Remote remote;
	libv::ui::UI ui;

private:
	libv::ui::Button button0;
	libv::ui::PanelLine panel_line;

public:
//	void create() {
//		remote.create();
//		remote.enableDebug();
//	}

	void render() {
//		auto gl = remote.queue();

//		gl.setClearColor(0.098f, 0.2f, 0.298f, 1.0f);
//		gl.clearColor();
//		gl.clearDepth();

//		sandbox.update(update_timer.timef_s());
//		sandbox.render();
		// <<< render is no longer called

//		ui.update();

//		remote.queue(std::move(gl));
//		remote.execute();
	}

//	void destroy() {
//		remote.destroy();
//	}

public:
	GenUIThemeFrame() :
		Frame("Gen UI Theme", WINDOW_WIDTH, WINDOW_HEIGHT) {
		setPosition(FramePosition::center_current_monitor);
		setOpenGLProfile(OpenGLProfile::core);
		setOpenGLVersion(3, 3);
		setOpenGLSamples(OpenGLSamples{4});
		ui.attach(*this);

		//

		button0.text("Hello World!");
		button0.event().submit([](libv::ui::Button& component, const libv::ui::EventSubmit& event) {
			(void) event;
			log_sandbox.info("Button pressed {}", component.path());
			component.text(component.text() + ".");
		});

		//

		button0.size(libv::ui::parse_size_or_throw("D, D"));

		panel_line.add(button0);

		ui.add(panel_line);

		//

		onKey.output([&](const libv::input::EventKey& e) {
			if (e.action == libv::input::Action::release)
				return;

			if (e.keycode == libv::input::Keycode::Escape)
				closeDefault();

			if (e.keycode == libv::input::Keycode::Space)
				pulse = !pulse;
		});
//		onContextCreate.output([&](const libv::frame::EventContextCreate&) {
//			create();
//		});
//		onContextUpdate.output([&](const libv::frame::EventContextUpdate&) {
//			render();
//		});
//		onContextDestroy.output([&](const libv::frame::EventContextDestroy&) {
//			destroy();
//		});
	}

	~GenUIThemeFrame() {
		disconnectAll();
	}
};

// Runner ------------------------------------------------------------------------------------------

int main(int, char**) {
	// For CLion console
	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");
	std::cout << libv::logger_stream;

	GenUIThemeFrame frame;
	frame.show();
	frame.join();

	return EXIT_SUCCESS;
}
