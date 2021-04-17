// Project: libv, File: app/space/space_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/ctrl/controls.hpp>
#include <libv/ctrl/feature_register.hpp>
#include <libv/frame/frame.hpp>
#include <libv/glr/attribute.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/procedural/sphere.hpp>
#include <libv/glr/program.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform_buffer.hpp>
#include <libv/log/log.hpp>
#include <libv/math/noise/white.hpp>
#include <libv/ui/component/canvas.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/ui.hpp>
#include <libv/utility/timer.hpp>
// std
#include <iostream>
#include <libv/ui/component/panel_full.hpp>
// pro
#include <space/camera.hpp>
#include <space/camera_behaviour.hpp>

//#include <libv/lua/lua.hpp>

//#include <libv/ui/component/panel_line.hpp>

//#include <libv/ui/style.hpp>
//#include <libv/input/event.hpp>
//#include <libv/input/input.hpp>
//#include <libv/parse/color.hpp>
//#include <libv/ui/component/button.hpp>
//#include <libv/ui/component/image.hpp>
//#include <libv/ui/component/input_field.hpp>
//#include <libv/ui/component/label_image.hpp>
//#include <libv/ui/component/panel_float.hpp>
//#include <libv/ui/component/panel_full.hpp>
//#include <libv/ui/component/panel_grid.hpp>
//#include <libv/ui/component/quad.hpp>
//#include <libv/ui/component/scroll_bar.hpp>
//#include <libv/ui/component/scroll_pane.hpp>
//#include <libv/ui/component/stretch.hpp>
//#include <libv/ui/context/context_ui.hpp>
//#include <libv/ui/parse/parse_size.hpp>
// ext
//#include <clip/clip.h>
//#include <fmt/format.h>


// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_space{libv::logger_stream, "space"};

// -------------------------------------------------------------------------------------------------

const auto shader_arrow_vs = R"(
#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 8) in vec2 vertexTexture0;

out vec3 fragmentNormal;
out vec2 fragmentTexture0;

layout(std140) uniform Sphere {
	mat4 MVPmat;
	mat4 Mmat;
	vec3 color;
};

void main() {
	gl_Position = MVPmat * vec4(vertexPosition, 1);
	fragmentNormal = normalize(vertexNormal);
	fragmentTexture0 = vertexTexture0;
}
)";

const auto shader_arrow_fs = R"(
#version 330 core

in vec3 fragmentNormal;
in vec2 fragmentTexture0;

out vec4 output;

layout(std140) uniform Sphere {
	mat4 MVPmat;
	mat4 Mmat;
	vec3 color;
};

void main() {
	output = vec4(fragmentTexture0, 1, 1);
}
)";

const auto shader_gizmo_vs = R"(
#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec4 vertexColor;

out vec4 fragmentColor;

layout(std140) uniform Sphere {
	mat4 MVPmat;
	mat4 Mmat;
	vec3 color;
};

void main() {
	gl_Position = MVPmat * vec4(vertexPosition, 1);
	fragmentColor = vertexColor;
}
)";

const auto shader_gizmo_fs = R"(
#version 330 core

in vec4 fragmentColor;

out vec4 output;

layout(std140) uniform Sphere {
	mat4 MVPmat;
	mat4 Mmat;
	vec3 color;
};

void main() {
	output = fragmentColor;
}
)";

const auto shader_background_vs = R"(
#version 330 core

layout(location = 0) in vec3 vertexPosition;

out vec2 fragmentTexture0;

void main() {
	gl_Position = vec4(vertexPosition, 1);
	fragmentTexture0 = vertexPosition.xy * 0.5 + 0.5;
}
)";

const auto shader_background_fs = R"(
#version 330 core

in vec2 fragmentTexture0;

out vec4 output;

uniform sampler2D textureNoise;
uniform vec2 noiseUVScale;
uniform vec4 noiseScale;
uniform vec4 noiseOffset;

//uniform vec4 color0;
//uniform vec4 color1;
//uniform vec4 colorCurve;

void main() {
	vec4 noise = texture2D(textureNoise, fragmentTexture0 * noiseUVScale, 0).rgba;
	output = noise * noiseScale + noiseOffset;
}
)";

constexpr auto attribute_position  = libv::glr::Attribute<0, libv::vec3f>{};
constexpr auto attribute_normal    = libv::glr::Attribute<1, libv::vec3f>{};
constexpr auto attribute_color0    = libv::glr::Attribute<2, libv::vec4f>{};
constexpr auto attribute_texture0  = libv::glr::Attribute<8, libv::vec2f>{};

const auto uniformBlock_sphere   = libv::glr::UniformBlockBinding{0, "Sphere"};

constexpr auto textureChannel_diffuse = libv::gl::TextureChannel{0};
constexpr auto textureChannel_normal  = libv::gl::TextureChannel{1};
constexpr auto textureChannel_pattern  = libv::gl::TextureChannel{7};

struct SphereUniformLayout {
	libv::glr::Uniform_mat4f MVPmat;
	libv::glr::Uniform_mat4f Mmat;
	libv::glr::Uniform_vec3f color;

	LIBV_REFLECTION_ACCESS(MVPmat);
	LIBV_REFLECTION_ACCESS(Mmat);
	LIBV_REFLECTION_ACCESS(color);
};
const auto arrow_layout = libv::glr::layout_std140<SphereUniformLayout>(uniformBlock_sphere);

// -------------------------------------------------------------------------------------------------

template <typename V0>
[[nodiscard]] constexpr inline auto length_and_dir(const V0& vec) noexcept {
	struct Result {
		typename V0::value_type length;
		V0 dir;
	};

	Result result;

	result.length = vec.length();
	result.dir = vec / result.length;

	return result;
}

struct ArrowOptions {
	float width = 0.1f;

//	float body_width = 0.1f;
//	float head_width = 0.3f;
//	float joint_cap = {round, cut, extent, none};
//	float end_cap = {round, none};
};

void draw_arrow(libv::glr::Mesh& mesh, std::vector<libv::vec3f> points, const ArrowOptions& options) {
	using vec3 = libv::vec3f;

	static constexpr vec3 up{0, 0, 1};

	if (points.size() < 2)
		return;

	auto position = mesh.attribute(attribute_position);
	auto normal = mesh.attribute(attribute_normal);
	auto texture0 = mesh.attribute(attribute_texture0);
	auto index = mesh.index();

	for (int32_t i = 0; i < static_cast<int32_t>(points.size()) - 1; i++) {
		const auto a = points[i];
		const auto b = points[i + 1];

		if (a == b)
			continue;

		const auto [length, dir] = length_and_dir(b - a);

		std::cout << length << " " << dir << std::endl;

		const auto right = cross(dir, up);
		const auto top = cross(dir, right);

		const auto side = right * options.width * 0.5f;

		position(a - side);
		position(a + side);
		position(b + side);
		position(b - side);

		normal(top);
		normal(top);
		normal(top);
		normal(top);

		texture0(0, 0);
		texture0(1, 0);
		texture0(1, 1);
		texture0(0, 1);

		index.quad(i * 4 + 0, i * 4 + 1, i * 4 + 2, i * 4 + 3);
	}
}

void draw_gizmo_lines(libv::glr::Mesh& mesh) {
	auto position = mesh.attribute(attribute_position);
	auto color0 = mesh.attribute(attribute_color0);
	auto index = mesh.index();

	position(0, 0, 0);
	position(1, 0, 0);
	position(0, 0, 0);
	position(0, 1, 0);
	position(0, 0, 0);
	position(0, 0, 1);

	color0(1, 0, 0, 1);
	color0(1, 0, 0, 1);
	color0(0, 1, 0, 1);
	color0(0, 1, 0, 1);
	color0(0, 0, 1, 1);
	color0(0, 0, 1, 1);

	index.line(0, 1);
	index.line(2, 3);
	index.line(4, 5);
}

// -------------------------------------------------------------------------------------------------

struct Background {
	libv::glr::Mesh mesh_background{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	libv::glr::Program program_background;
	libv::glr::Texture2D::R8_G8_B8 background_texture_pattern;
	libv::glr::Uniform_vec2f background_uniform_noiseUVScale;
	libv::glr::Uniform_vec4f background_uniform_noiseScale;
	libv::glr::Uniform_vec4f background_uniform_noiseOffset;
	libv::glr::Uniform_texture background_uniform_textureNoise;

	static constexpr libv::vec2i noise_size = {128, 128};

	Background() {
		program_background.vertex(shader_background_vs);
		program_background.fragment(shader_background_fs);
		program_background.assign(background_uniform_noiseUVScale, "noiseUVScale");
		program_background.assign(background_uniform_noiseScale, "noiseScale");
		program_background.assign(background_uniform_noiseOffset, "noiseOffset");
		program_background.assign(background_uniform_textureNoise, "textureNoise", textureChannel_pattern);
		// TODO P1: Switch to blue noise once implemented
		const auto tex_data = libv::noise_white_2D_3uc(0x5EED, noise_size.x, noise_size.y);

		background_texture_pattern.storage(1, noise_size);
		background_texture_pattern.image(0, {0, 0}, noise_size, tex_data.data());
		background_texture_pattern.set(libv::gl::MagFilter::Nearest);
		background_texture_pattern.set(libv::gl::MinFilter::Nearest);
		background_texture_pattern.set(libv::gl::Wrap::Repeat, libv::gl::Wrap::Repeat);

		{
			auto position = mesh_background.attribute(attribute_position);
			auto index = mesh_background.index();

			position(-1, -1, 0);
			position( 1, -1, 0);
			position( 1,  1, 0);
			position(-1,  1, 0);

			index.quad(0, 1, 2, 3);
		}
	}

	void render(libv::glr::Queue& gl, libv::vec2f canvas_size) {
		const auto s_guard = gl.state.push_guard();

		gl.state.disableDepthMask();
		gl.state.disableDepthTest();
		gl.state.polygonModeFill();

		gl.program(program_background);
		// TODO P1: Update shader to operate on color-noise uniforms
//		const auto bg_noise = 1.f;
		const auto bg_noise = 5.f / 255.f;
		const auto bg_color = libv::vec4f(0.098f, 0.2f, 0.298f, 1.0f) - bg_noise * 0.5f;
		gl.uniform(background_uniform_noiseUVScale, canvas_size / noise_size.cast<float>());
		gl.uniform(background_uniform_noiseScale, libv::vec4f(bg_noise, bg_noise, bg_noise, 0));
		gl.uniform(background_uniform_noiseOffset, bg_color);
//		gl.uniform(background_uniform_noiseScale, libv::vec4f(0.1f, 0.1f, 0.1f, 1));
//		gl.uniform(background_uniform_noiseOffset, libv::vec4f(0.6f, 0.6f, 0.6f, 0));
//		gl.uniform(background_uniform_noiseScale, libv::vec4f(0, 0, 0, 0));
//		gl.uniform(background_uniform_noiseOffset, libv::vec4f(0, 0, 0, 0));
		gl.texture(background_texture_pattern, textureChannel_pattern);
		gl.render(mesh_background);
	}
};

// -------------------------------------------------------------------------------------------------

struct SpaceCanvas : libv::ui::Canvas {
	app::CameraPlayer& camera;

	float angle = 0.0f;
	float time = 0.0f;

	libv::glr::Mesh mesh_gizmo{libv::gl::Primitive::Lines, libv::gl::BufferUsage::StaticDraw};
	libv::glr::Program program_gizmo;

	Background background;

	libv::glr::Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	libv::glr::Program program;
	libv::glr::UniformBuffer arrow_uniforms{libv::gl::BufferUsage::StreamDraw};

	explicit SpaceCanvas(app::CameraPlayer& camera) :
		camera(camera) {

		program.vertex(shader_arrow_vs);
		program.fragment(shader_arrow_fs);
		program.block_binding(uniformBlock_sphere);

		program_gizmo.vertex(shader_gizmo_vs);
		program_gizmo.fragment(shader_gizmo_fs);
		program_gizmo.block_binding(uniformBlock_sphere);

		ArrowOptions options;

		std::vector<libv::vec3f> points{{0, 0, 0}, {1, 0.5, 0.5}, {1, 1, 0}, {1, 2, 2}, {-1, -1, -1}};
		for (int i = 0; i < 60; i++) {
			const auto r = i / 30.0;
			const auto x = std::sin(libv::deg_to_rad(i * 15.0)) * r;
			const auto y = std::cos(libv::deg_to_rad(i * 15.0)) * r;
			points.emplace_back(x, y, 0);
		}
		for (int i = 60; i < 120; i++) {
			const auto r = 2.0 - (i - 60) / 30.0 * 0.5;
			const auto x = std::sin(libv::deg_to_rad(i * 15.0)) * r;
			const auto y = std::cos(libv::deg_to_rad(i * 15.0)) * r;
			const auto z = std::sin(libv::deg_to_rad((i - 60) * 30.0)) * 0.25;
			points.emplace_back(x, y, z);
		}

		draw_gizmo_lines(mesh_gizmo);
		draw_arrow(mesh, points, options);
	}

	virtual void update(libv::ui::time_duration delta_time) override {
		const auto dtf = static_cast<float>(delta_time.count());
		angle = std::fmod(angle + 5.0f * dtf, 360.0f);
		time += dtf;

		// TODO P2: Value tracking UI component for debugging
//		libv::ui::value_tracker tracker(600 /*sample*/, 0.15, 0.85);
//		value_tracker(160);
//		value_tracker.pause();
//		value_tracker.resume();
//		value_tracker("camera.orbit_point", camera.orbit_point());
//		value_tracker("camera.orbit_distance", camera.orbit_distance());
//		value_tracker("camera.rotations", camera.rotations());
//		value_tracker.differential("camera.orbit_point", camera.orbit_point());
//		value_tracker.differential_focused("camera.orbit_point", camera.orbit_point(), 0.15, 0.85);
//		value_tracker.differential_focused_timed("camera.orbit_point", camera.orbit_point(), 0.15, 0.85);
	}

	virtual void render(libv::glr::Queue& gl) override {
		const auto s_guard = gl.state.push_guard();

		gl.state.enableDepthTest();

		gl.projection = camera.projection(canvas_size);
		gl.view = camera.view();
//		gl.projection = libv::mat4f::perspective(1.f, 1.f * canvas_size.x / canvas_size.y, 0.1f, 1000.f);
//		gl.view = libv::mat4f::lookAt({2.f, 2.f, 1.2f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 1.f});
//		gl.view.rotate(libv::Degrees{angle}, 0.f, 0.f, 1.f);
		gl.model = libv::mat4f::identity();

		background.render(gl, canvas_size);

		{
			auto uniforms = arrow_uniforms.block_unique(arrow_layout);
			uniforms[arrow_layout.MVPmat] = gl.mvp();
			uniforms[arrow_layout.Mmat] = gl.model;
			uniforms[arrow_layout.color] = libv::vec3f(1.0f, 1.0f, 1.0f);

			gl.program(program_gizmo);
			gl.uniform(std::move(uniforms));
			gl.render(mesh_gizmo);
		}

		{
			const auto m_guard = gl.model.push_guard();

//			std::cout << "Eye: " << camera.eye() << " Cam: " << camera.orbit_point() << " rot: " << camera.rotations() << " dir: " << camera.direction() << std::endl;
			gl.model.translate(camera.orbit_point());
			gl.model.scale(0.2f);

			auto uniforms = arrow_uniforms.block_unique(arrow_layout);
			uniforms[arrow_layout.MVPmat] = gl.mvp();
			uniforms[arrow_layout.Mmat] = gl.model;
			uniforms[arrow_layout.color] = libv::vec3f(1.0f, 1.0f, 1.0f);

			gl.program(program_gizmo);
			gl.uniform(std::move(uniforms));
			gl.render(mesh_gizmo);
		}

		{
			auto uniforms = arrow_uniforms.block_unique(arrow_layout);
			uniforms[arrow_layout.MVPmat] = gl.mvp();
			uniforms[arrow_layout.Mmat] = gl.model;
			uniforms[arrow_layout.color] = libv::vec3f(1.0f, 1.0f, 1.0f);

			gl.program(program);
			gl.uniform(std::move(uniforms));
			gl.render(mesh);
		}
	}
};

// -------------------------------------------------------------------------------------------------

// TODO P1: Controls camera should only be placed into context if the canvas is focused
// TODO P2: UI Canvas, find a better API, let the component take the ownership of the canvas
// TODO P3: Arrow strip control from lua (or something lua related) (With auto reload and everything)
// TODO P4: Nicer arrow strips, animation

int main() {
	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");

	std::cout << libv::logger_stream;
	log_space.info("Hello Space!");

	libv::Frame frame("Space", 1280, 800);
	libv::ui::UI ui;
	libv::ctrl::Controls controls;

	app::CameraBehaviour::register_controls(controls);
	app::CameraBehaviour::bind_default_controls(controls);

	app::CameraPlayer camera;
	camera.look_at({2.f, 2.f, 1.2f}, {0.f, 0.f, 0.f});
	controls.context_enter<app::BaseCameraOrbit>(&camera); // TODO P4: <app::BaseCameraOrbit> Question mark? Context variables and inheritance?

	frame.setPosition(libv::Frame::FramePosition::center_current_monitor);
	frame.setOpenGLProfile(libv::Frame::OpenGLProfile::core);
	frame.setOpenGLVersion(3, 3);
	frame.setOpenGLSamples(libv::Frame::OpenGLSamples{4});
//	frame.setOpenGLRefreshRate(libv::Frame::OpenGLRefreshRate{1});

	// TODO P1: Timer management should be smoother with control and frame attachment
	libv::Timer timer;
	frame.onContextUpdate.output([&](const auto&) {
		controls.update(timer.time());
	});

	frame.onKey.output([&](const libv::input::EventKey& e) {
		if (e.keycode == libv::input::Keycode::Escape)
			frame.closeForce();
	});

	ui.attach(frame);
	controls.attach(frame);

	SpaceCanvas space(camera);

	{
		libv::ui::PanelFull layers;
		libv::ui::Label label;
		label.align_horizontal(libv::ui::AlignHorizontal::left);
		label.align_vertical(libv::ui::AlignVertical::bottom);
//		label.align_horizontal(libv::ui::AlignHorizontal::center);
//		label.align_vertical(libv::ui::AlignVertical::center);
		label.text("SPACE!");
		label.font_color({.7f, 0.2f, 0.4f, 1.f});

		libv::ui::CanvasAdaptor canvas;
		canvas.adopt(&space);
//		libv::ui::Canvas2<SpaceCanvas> canvas;

		layers.add(label);
		layers.add(canvas);
		ui.add(layers);
	}

	frame.show();
	frame.join();

	return 0;
}
