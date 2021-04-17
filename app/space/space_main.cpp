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
layout(location = 15) in vec4 vertex_SP_SS_TP_TS; // SegmentPosition, SegmentSize, TotalPosition, TotalSize

out vec3 fragmentNormal;
out vec2 fragmentTexture0;
out float fragmentSegmentPosition;
out float fragmentSegmentSize;
out float fragmentTotalPosition;
out float fragmentTotalSize;

layout(std140) uniform Sphere {
	mat4 MVPmat;
	mat4 Mmat;
	vec3 color;
};

void main() {
	float vertexSegmentPosition = vertex_SP_SS_TP_TS.x;
	float vertexSegmentSize = vertex_SP_SS_TP_TS.y;
	float vertexTotalPosition = vertex_SP_SS_TP_TS.z;
	float vertexTotalSize = vertex_SP_SS_TP_TS.w;

	gl_Position = MVPmat * vec4(vertexPosition, 1);
	fragmentNormal = normalize(vertexNormal);
	fragmentTexture0 = vertexTexture0;

	fragmentSegmentPosition = vertexSegmentPosition;
	fragmentSegmentSize = vertexSegmentSize;
	fragmentTotalPosition = vertexTotalPosition;
	fragmentTotalSize = vertexTotalSize;
}
)";

const auto shader_arrow_fs = R"(
#version 330 core

in vec3 fragmentNormal;
in vec2 fragmentTexture0;
in float fragmentSegmentPosition;
in float fragmentSegmentSize;
in float fragmentTotalPosition;
in float fragmentTotalSize;

out vec4 output;

layout(std140) uniform Sphere {
	mat4 MVPmat;
	mat4 Mmat;
	vec3 color;
};

float saturate(in float v) {
	return clamp(v, 0.0, 1.0);
}

/// Anti-aliased mask creation from SDF with zero as limit
float mask_aa(in float sdf) {
	// Method A: fwidth and smoothstep
	// Best result
	// Origin: https://www.ronja-tutorials.com/post/034-2d-sdf-basics/
	float delta = fwidth(sdf) * 0.5;
	float mask_aa = smoothstep(delta, -delta, sdf);

	// Method B: division (Acegikmo)
	// Issue: Enlarges the mask with half a pixel and therefore causes some artifacts.
	// Origin: https://www.youtube.com/watch?v=mL8U8tIiRRg
	// float delta = fwidth(sdf);
	// float mask_aa = 1 - clamp(sdf / delta, 0, 1);

	// Method C: division with half pixel correction
	// Generates a bit blurrier/smoother edges, a good alternative to Method A
	// Based on: https://www.youtube.com/watch?v=mL8U8tIiRRg
	// float delta = fwidth(sdf);
	// float mask_aa = 1 - clamp(sdf / delta + 0.5, 0, 1);

	// Method D: step, the anti-method
	// To just disable AA
	// float mask_aa = step(sdf, 0);

	return mask_aa;
}

float sdTriangle(in vec2 p, in vec2 p0, in vec2 p1, in vec2 p2) {
	vec2 e0 = p1 - p0;
	vec2 e1 = p2 - p1;
	vec2 e2 = p0 - p2;

	vec2 v0 = p - p0;
	vec2 v1 = p - p1;
	vec2 v2 = p - p2;

	vec2 pq0 = v0 - e0 * clamp(dot(v0, e0) / dot(e0, e0), 0.0, 1.0);
	vec2 pq1 = v1 - e1 * clamp(dot(v1, e1) / dot(e1, e1), 0.0, 1.0);
	vec2 pq2 = v2 - e2 * clamp(dot(v2, e2) / dot(e2, e2), 0.0, 1.0);

    float s = e0.x * e2.y - e0.y * e2.x;
    vec2 d = min(min(vec2(dot(pq0, pq0), s * (v0.x * e0.y - v0.y * e0.x)),
                     vec2(dot(pq1, pq1), s * (v1.x * e1.y - v1.y * e1.x))),
                     vec2(dot(pq2, pq2), s * (v2.x * e2.y - v2.y * e2.x)));

	return -sqrt(d.x) * sign(d.y);
}

void main() {
	const vec4 color_body = vec4(0.60, 0.60, 0.60, 1);
	const vec4 color_edge = vec4(0.85, 0.85, 0.85, 1);
	const vec4 color_head = vec4(0.85, 0.30, 0.30, 1);
	const float body_size = 0.400;
	const float edge_size = 0.075;
	const float head_size = 0.125;

	float distance_from_middle = abs(fragmentTexture0.x - 0.5);

	output = vec4(color_body.rgb, 0);

	float body_sdf = distance_from_middle - body_size * 0.5;
	output = mix(output, color_body, mask_aa(body_sdf));

	float edge_sdf = distance_from_middle - body_size * 0.5 + edge_size;
	output = mix(output, color_edge, mask_aa(edge_sdf));

	float head_x = fragmentTexture0.x;
	float head_y = fragmentSegmentSize - fragmentSegmentPosition - head_size;
	float head_sdf = sdTriangle(vec2(head_x, head_y), vec2(0.5, -head_size), vec2(0, 0), vec2(1, 0));
	output = mix(output, color_head, mask_aa(head_sdf));
}
)";

// TODO P1: In shader_arrow_fs body_size/edge_size is UV but head_size is world space. Unify them
// TODO P1: Organize vertex_SP_SS_TP_TS to SS_SP_SE_TS s.start, s.position, s.end, t.size or with some more sane name
// TODO P1: Move shaders to files
// TODO P1: Move mask_aa into its own file
// TODO P1: Shader hot loader
// TODO P2: No sdTriangle (at all) solve the problem without triangle sdf
// TODO P4: Shader Importer

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

const auto shader_grid_vs = R"(
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

const auto shader_grid_fs = R"(
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
constexpr auto attribute_normal	= libv::glr::Attribute<1, libv::vec3f>{};
constexpr auto attribute_color0	= libv::glr::Attribute<2, libv::vec4f>{};
constexpr auto attribute_texture0  = libv::glr::Attribute<8, libv::vec2f>{};
constexpr auto attribute_custom0	= libv::glr::Attribute<15, libv::vec4f>{};

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
	using f = float;

	static constexpr vec3 up{0, 0, 1};

	if (points.size() < 2)
		return;

	auto position = mesh.attribute(attribute_position);
	auto normal = mesh.attribute(attribute_normal);
	auto texture0 = mesh.attribute(attribute_texture0);
	auto sp_ss_tp_ts = mesh.attribute(attribute_custom0); // SegmentPosition, SegmentSize, TotalPosition, TotalSize
	auto index = mesh.index();

	f total_position = 0;
	f total_size = 0;

	for (int32_t i = 0; i < static_cast<int32_t>(points.size()) - 1; i++) {
		const auto a = points[i];
		const auto b = points[i + 1];

		total_size += (b - a).length();
	}

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

		sp_ss_tp_ts(0, length, total_position, total_size);
		sp_ss_tp_ts(0, length, total_position, total_size);
		sp_ss_tp_ts(length, length, total_position, total_size);
		sp_ss_tp_ts(length, length, total_position, total_size);

		index.quad(i * 4 + 0, i * 4 + 1, i * 4 + 2, i * 4 + 3);

		total_position += length;
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
		// TODO P1: Update shader to operate on color and noise uniforms
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

//struct Grid {
//	libv::glr::Mesh mesh_lines{libv::gl::Primitive::Lines, libv::gl::BufferUsage::StaticDraw};
//	libv::glr::Program program_lines;
////	libv::glr::Texture2D::R8_G8_B8 background_texture_pattern;
////	libv::glr::Uniform_vec2f background_uniform_noiseUVScale;
////	libv::glr::Uniform_texture background_uniform_textureNoise;
//	libv::glr::Uniform_vec4f uniform_color_x;
//	libv::glr::Uniform_vec4f uniform_color_y;
//	libv::glr::Uniform_vec4f uniform_color_z;
//	libv::glr::Uniform_vec4f uniform_color_line_low;
//	libv::glr::Uniform_vec4f uniform_color_line_mid;
//	libv::glr::Uniform_vec4f uniform_color_line_high;
//
////	float far;
//
//	static constexpr libv::vec2i noise_size = {128, 128};
//
//	Grid() {
////		program_background.vertex(shader_background_vs);
////		program_background.fragment(shader_background_fs);
////		program_background.assign(background_uniform_noiseUVScale, "noiseUVScale");
////		program_background.assign(background_uniform_noiseScale, "noiseScale");
////		program_background.assign(background_uniform_noiseOffset, "noiseOffset");
////		program_background.assign(background_uniform_textureNoise, "textureNoise", textureChannel_pattern);
////		// TODO P1: Switch to blue noise once implemented
////		const auto tex_data = libv::noise_white_2D_3uc(0x5EED, noise_size.x, noise_size.y);
////
////		background_texture_pattern.storage(1, noise_size);
////		background_texture_pattern.image(0, {0, 0}, noise_size, tex_data.data());
////		background_texture_pattern.set(libv::gl::MagFilter::Nearest);
////		background_texture_pattern.set(libv::gl::MinFilter::Nearest);
////		background_texture_pattern.set(libv::gl::Wrap::Repeat, libv::gl::Wrap::Repeat);
//
//		{
//			auto position = mesh_background.attribute(attribute_position);
//			auto index = mesh_background.index();
//
//			int count = 5000;
//			float limit = 5000;
//
//			// Minor lines - 1 px dark
//			// Major lines - 2 px light
//			// Axis lines  - 2 px colored
//			// Auto tessalate Minors -> Major with new Minors
//			//      on smooth
//			// Min size is 0.001
//			// Fade based on inclination
//
//			for (int i = 0; i < count; ++i) {
//				position(-limit, limit, 0);
//				position(limit, limit, 0);
//
//				position(limit, -limit, 0);
//				position(limit, limit, 0);
//			}
//
//			for (int i = 0; i < count; ++i) {
//				position(-limit, limit, 0);
//				position(limit, limit, 0);
//
//				position(limit, -limit, 0);
//				position(limit, limit, 0);
//			}
//
////			position(-1, -1, 0);
////			position( 1, -1, 0);
////			position( 1,  1, 0);
////			position(-1,  1, 0);
//
//			index.quad(0, 1, 2, 3);
//		}
//	}
//
//	void render(libv::glr::Queue& gl) {
////		const auto s_guard = gl.state.push_guard();
////
////		gl.state.disableDepthMask();
////		gl.state.disableDepthTest();
////		gl.state.polygonModeFill();
////
////		gl.program(program_background);
////		// TODO P1: Update shader to operate on color and noise uniforms
//////		const auto bg_noise = 1.f;
////		const auto bg_noise = 5.f / 255.f;
////		const auto bg_color = libv::vec4f(0.098f, 0.2f, 0.298f, 1.0f) - bg_noise * 0.5f;
////		gl.uniform(background_uniform_noiseUVScale, canvas_size / noise_size.cast<float>());
////		gl.uniform(background_uniform_noiseScale, libv::vec4f(bg_noise, bg_noise, bg_noise, 0));
////		gl.uniform(background_uniform_noiseOffset, bg_color);
//////		gl.uniform(background_uniform_noiseScale, libv::vec4f(0.1f, 0.1f, 0.1f, 1));
//////		gl.uniform(background_uniform_noiseOffset, libv::vec4f(0.6f, 0.6f, 0.6f, 0));
//////		gl.uniform(background_uniform_noiseScale, libv::vec4f(0, 0, 0, 0));
//////		gl.uniform(background_uniform_noiseOffset, libv::vec4f(0, 0, 0, 0));
////		gl.texture(background_texture_pattern, textureChannel_pattern);
////		gl.render(mesh_background);
//	}
//};

// -------------------------------------------------------------------------------------------------

struct SpaceCanvas : libv::ui::Canvas {
	app::CameraPlayer& camera;

	float angle = 0.0f;
	float time = 0.0f;

	int32_t test_mode = 0;

	libv::glr::Mesh mesh_gizmo{libv::gl::Primitive::Lines, libv::gl::BufferUsage::StaticDraw};
	libv::glr::Program program_gizmo;

	Background background;

	libv::glr::Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	libv::glr::Program program;
	libv::glr::UniformBuffer arrow_uniforms{libv::gl::BufferUsage::StreamDraw};
	libv::glr::Uniform_int32 uniform_test_mode;


	explicit SpaceCanvas(app::CameraPlayer& camera) :
		camera(camera) {

		program.vertex(shader_arrow_vs);
		program.fragment(shader_arrow_fs);
		program.assign(uniform_test_mode, "test_mode");
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
			gl.state.enableBlend();
			gl.state.blendSrc_SourceAlpha();
			gl.state.blendDst_One_Minus_SourceAlpha();

			// TODO P1: With glsl generated shapes depth writing is not a good idea
			//		  Also, the current mode would require glsl fwidth AA, possible but cumbersome
			gl.state.disableDepthMask();

			auto uniforms = arrow_uniforms.block_unique(arrow_layout);
			uniforms[arrow_layout.MVPmat] = gl.mvp();
			uniforms[arrow_layout.Mmat] = gl.model;
			uniforms[arrow_layout.color] = libv::vec3f(1.0f, 1.0f, 1.0f);

			gl.program(program);
			gl.uniform(std::move(uniforms));
			gl.uniform(uniform_test_mode, test_mode);
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
	camera.look_at({1.6f, 1.6f, 1.2f}, {0.5f, 0.5f, 0.f});
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

	ui.attach(frame);
	controls.attach(frame);

	SpaceCanvas space(camera);

	frame.onKey.output([&](const libv::input::EventKey& e) {
		if (e.keycode == libv::input::Keycode::Escape)
			frame.closeForce();

		// TODO P1: Shortcut to save camera position and reload it upon restart
		//          > Requires persistence
		// TODO P1: Remember auto runtime hook options
		//          > Requires persistence
		// TODO P1: Auto runtime hook option for random uniform variables
		if (e.keycode == libv::input::Keycode::C && e.action != libv::input::Action::release) {
			const int32_t mode_count = 4;
			if (frame.isKeyPressed(libv::input::Keycode::ShiftLeft) || frame.isKeyPressed(libv::input::Keycode::ShiftRight))
				space.test_mode = space.test_mode == 0 ? mode_count - 1 : space.test_mode - 1;
			else
				space.test_mode = (space.test_mode + 1) % mode_count;
		}
		if (e.keycode == libv::input::Keycode::Backtick)
			space.test_mode = 0;
		if (e.keycode == libv::input::Keycode::Num1)
			space.test_mode = 1;
		if (e.keycode == libv::input::Keycode::Num2)
			space.test_mode = 2;
		if (e.keycode == libv::input::Keycode::Num3)
			space.test_mode = 3;
		log_space.info("Test mode: {}", space.test_mode);
	});

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
