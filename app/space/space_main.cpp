// Project: libv, File: app/space/space_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/algo/adjacent_pairs.hpp>
#include <libv/ctrl/controls.hpp>
#include <libv/ctrl/feature_register.hpp>
#include <libv/frame/frame.hpp>
#include <libv/glr/attribute.hpp>
#include <libv/glr/layout_to_string.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/procedural/sphere.hpp>
#include <libv/glr/procedural/cube.hpp>
#include <libv/glr/program.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform_buffer.hpp>
#include <libv/log/log.hpp>
#include <libv/math/noise/white.hpp>
#include <libv/rev/shader.hpp>
#include <libv/rev/shader_loader.hpp>
#include <libv/ui/component/canvas.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_full.hpp>
#include <libv/ui/settings.hpp>
#include <libv/ui/ui.hpp>
#include <libv/utility/hex_dump.hpp>
#include <libv/utility/timer.hpp>
// std
#include <iostream>
// pro
#include <space/camera.hpp>
#include <space/camera_behaviour.hpp>
#include <space/icons.hpp>

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

// TODO P1: In shader_arrow_fs body_size/edge_size is UV but head_size is world space. Unify them
// TODO P1: Organize vertex_SP_SS_TP_TS to SS_SP_SE_TS s.start, s.position, s.end, t.size or with some more sane name

// TODO P2: libv.glr: Shader automated block binding by watching the includes
//          So this should work:
//          shader_loader.register_block<UniformLayoutMatrices>("block/sphere.glsl");
//          Note that this also could generate the file block/sphere.glsl (OR just be an in memory resource)
//          And this would be called on any program that includes block/sphere.glsl
//          program.block_binding(uniformBlock_matrices)
//          | issue: some struct might have already been defined, so block to string might have to skip them
//                  This mean tracking of structs OR pushing the problem back to the include system pragma once solution
//                  With additional mapping and includes to struct/my_struct_that_is_in_a_block.glsl


constexpr auto attribute_position  = libv::glr::Attribute<0, libv::vec3f>{};
constexpr auto attribute_normal	= libv::glr::Attribute<1, libv::vec3f>{};
constexpr auto attribute_color0	= libv::glr::Attribute<2, libv::vec4f>{};
constexpr auto attribute_texture0 = libv::glr::Attribute<8, libv::vec2f>{};
constexpr auto attribute_custom0 = libv::glr::Attribute<15, libv::vec4f>{};

constexpr auto textureChannel_diffuse = libv::gl::TextureChannel{0};
constexpr auto textureChannel_normal  = libv::gl::TextureChannel{1};
constexpr auto textureChannel_pattern  = libv::gl::TextureChannel{7};

struct UniformLayoutMatrices {
	libv::glr::Uniform_mat4f matMVP;
	libv::glr::Uniform_mat4f matP;
	libv::glr::Uniform_mat4f matM;
	libv::glr::Uniform_vec3f eye;

	LIBV_REFLECTION_ACCESS(matMVP);
	LIBV_REFLECTION_ACCESS(matP);
	LIBV_REFLECTION_ACCESS(matM);
	LIBV_REFLECTION_ACCESS(eye);
};
const auto uniformBlock_matrices = libv::glr::UniformBlockBinding{0, "Matrices"};
const auto layout_matrices = libv::glr::layout_std140<UniformLayoutMatrices>(uniformBlock_matrices);

// -------------------------------------------------------------------------------------------------

//template <typename V0>
//[[nodiscard]] constexpr inline auto length_and_dir(const V0& vec) noexcept {
//	struct Result {
//		typename V0::value_type length;
//		V0 dir;
//	};
//
//	Result result;
//
//	result.length = vec.length();
//	result.dir = vec / result.length;
//
//	return result;
//}

// -------------------------------------------------------------------------------------------------

libv::rev::ShaderLoader shader_manager("shader/");

// -------------------------------------------------------------------------------------------------

struct UniformsBackground {
//	libv::glr::Uniform_vec2f noiseUVScale;
//	libv::glr::Uniform_vec4f noiseScale;
//	libv::glr::Uniform_vec4f noiseOffset;
//	libv::glr::Uniform_texture textureNoise;

	libv::glr::Uniform_texture texture_noise;
	libv::glr::Uniform_vec2f render_resolution;
	libv::glr::Uniform_vec4f noise_scale;
	libv::glr::Uniform_vec4f base_color;

	template <typename Access> void access_uniforms(Access& access) {
		access(texture_noise, "texture_noise", textureChannel_pattern);
		access(render_resolution, "render_resolution");
		access(base_color, "base_color");
		access(noise_scale, "noise_scale");
	}
};

struct UniformsTestMode {
	libv::glr::Uniform_int32 test_mode;

	template <typename Access> void access_uniforms(Access& access) {
		access(test_mode, "test_mode", 0);
	}
};

struct UniformsColor {
	libv::glr::Uniform_vec4f base_color;

	template <typename Access> void access_uniforms(Access& access) {
		access(base_color, "base_color");
	}
};

struct UniformsCommandArrow {
	libv::glr::Uniform_int32 test_mode;
	libv::glr::Uniform_float time;
	libv::glr::Uniform_vec2f render_resolution;
	libv::glr::Uniform_vec4f color;

	template <typename Access> void access_uniforms(Access& access) {
		access(color, "color");
		access(render_resolution, "render_resolution");
		access(test_mode, "test_mode", 0);
		access(time, "time", 0.f);
	}
};

using ShaderBackground = libv::rev::Shader<UniformsBackground>;
using ShaderCommandArrow = libv::rev::Shader<UniformsCommandArrow>;

using ShaderColor = libv::rev::Shader<UniformsColor>;
using ShaderTestMode = libv::rev::Shader<UniformsTestMode>;
//using Shader = libv::rev::Shader<>;

// =================================================================================================

// TODO P1: kill these ones, aka implement 'program' uniform block
float global_time = 0.0f;
int32_t global_test_mode = 0;

// =================================================================================================

struct Background {
	libv::glr::Mesh mesh_background{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

	ShaderBackground shader{shader_manager, "editor_background.vs", "editor_background.fs"};
	libv::glr::Texture2D::R8_G8_B8 background_texture_pattern;

	static constexpr libv::vec2i noise_size = {128, 128};

	Background() {
		// TODO P1: Switch to blue noise once implemented
		//  		| It will not be implemented anytime soon so burn in a couple of textures from it
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
		gl.program(shader.program());
		const auto bg_noise = libv::vec4f(1, 1, 1, 0) * (5.f / 255.f);
		const auto bg_color = libv::vec4f(0.098f, 0.2f, 0.298f, 1.0f);
		gl.uniform(shader.uniform().render_resolution, canvas_size);
		gl.uniform(shader.uniform().noise_scale, bg_noise);
		gl.uniform(shader.uniform().base_color, bg_color);
		gl.texture(background_texture_pattern, textureChannel_pattern);
		gl.render(mesh_background);
	}
};

struct CommandArrow {
	libv::glr::Mesh mesh{libv::gl::Primitive::Lines, libv::gl::BufferUsage::StaticDraw};
	ShaderCommandArrow shader_arrow{shader_manager, "command_arrow.vs", "command_arrow.gs", "command_arrow.fs"};

public:
	CommandArrow() {
		// TODO P1: libv.rev: Auto block binding based on include detection | intermediate step: integrate to shader function type / override
		shader_arrow.program().block_binding(uniformBlock_matrices);

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

		draw_arrow(mesh, points);
	}

	void draw_arrow(libv::glr::Mesh& mesh, std::vector<libv::vec3f> points) {
		if (points.size() < 2)
			return;

		auto position = mesh.attribute(attribute_position);
		auto color0 = mesh.attribute(attribute_color0);
		auto sp_ss_tp_ts = mesh.attribute(attribute_custom0); // SegmentPosition, SegmentSize, TotalPosition, TotalSize
		auto index = mesh.index();

		float total_length = 0;
		float current_length = 0;

		libv::algo::adjacent_pairs(points, [&](auto a, auto b) {
			total_length += (b - a).length();
		});

		libv::glr::VertexIndex i = 0;
		libv::algo::adjacent_pairs(points, [&](auto a, auto b) {
			if (a == b) // Sanity check
				return;

			const auto length = (b - a).length();

			position(a);
			position(b);

			color0(1, 1, 1, 1);
			color0(1, 1, 1, 1);

			sp_ss_tp_ts(0, length, current_length, total_length);
			sp_ss_tp_ts(length, length, current_length, total_length);

			index.line(i + 0, i + 1);
			i += 2;

			current_length += length;
		});
	}

	void render(libv::glr::Queue& gl, libv::vec2f canvas_size, libv::glr::UniformBuffer& uniform_stream) const {
		// TODO P2: This will require to re upload to VAO every render
		//  		but it could be optimized if we give it a 'pretend' start offset
		//  		single uniform value that adjust the 'fake' starting point (not only the first section could have this)
		//			This also means VAO update only required on NEW command (SUPER GOOD), every normal render only changes some uniform
		//			| MAJOR ISSUE: Command arrows that move, like follow would break this system
		//				Resolution ideas: indirections?, uniform array as coordinates?, VBA update is okey?
		//			and/or Command arrows could be merged into a single VAO and use sub-meshes to render

		auto uniforms = uniform_stream.block_unique(layout_matrices);
		uniforms[layout_matrices.matMVP] = gl.mvp();
		uniforms[layout_matrices.matM] = gl.model;
		uniforms[layout_matrices.matP] = gl.projection;
		uniforms[layout_matrices.eye] = gl.eye();

		gl.program(shader_arrow.program());
		gl.uniform(std::move(uniforms));
		gl.uniform(shader_arrow.uniform().color, libv::vec4f(1.0f, 1.0f, 1.0f, 1.0f));
		gl.uniform(shader_arrow.uniform().render_resolution, canvas_size);
		gl.uniform(shader_arrow.uniform().test_mode, global_test_mode);
		gl.uniform(shader_arrow.uniform().time, global_time);
		gl.render(mesh);
	}
};

struct Gizmo {
	libv::glr::Mesh mesh{libv::gl::Primitive::Lines, libv::gl::BufferUsage::StaticDraw};
	ShaderTestMode shader{shader_manager, "editor_gizmo.vs", "editor_gizmo.fs"};

public:
	Gizmo() {
		// TODO P1: libv.rev: Auto block binding based on include detection | intermediate step: integrate to shader function type / override
		shader.program().block_binding(uniformBlock_matrices);

		draw_gizmo_lines(mesh);
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

	void render(libv::glr::Queue& gl, libv::glr::UniformBuffer& uniform_stream) {
		auto uniforms = uniform_stream.block_unique(layout_matrices);
		uniforms[layout_matrices.matMVP] = gl.mvp();
		uniforms[layout_matrices.matM] = gl.model;
		uniforms[layout_matrices.matP] = gl.projection;
		uniforms[layout_matrices.eye] = gl.eye();

		gl.program(shader.program());
		gl.uniform(std::move(uniforms));
		gl.render(mesh);
	}
};

struct Grid {
	libv::glr::Mesh mesh_grid{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	ShaderTestMode shader{shader_manager, "editor_grid_plane.vs", "editor_grid_plane.fs"};

public:
	Grid() {
		// TODO P1: libv.rev: Auto block binding based on include detection | intermediate step: integrate to shader function type / override
		shader.program().block_binding(uniformBlock_matrices);

		{
			auto position = mesh_grid.attribute(attribute_position);
			auto index = mesh_grid.index();

			position(-1, -1, 0);
			position(+1, -1, 0);
			position(+1, +1, 0);
			position(-1, +1, 0);

			index.quad(0, 1, 2, 3); // Front face quad
			index.quad(0, 3, 2, 1); // Back face quad
		}
	}

	void render(libv::glr::Queue& gl, libv::glr::UniformBuffer& uniform_stream) {
		auto uniforms = uniform_stream.block_unique(layout_matrices);
		uniforms[layout_matrices.matMVP] = gl.mvp();
		uniforms[layout_matrices.matM] = gl.model;
		uniforms[layout_matrices.matP] = gl.projection;
		uniforms[layout_matrices.eye] = gl.eye();

		gl.program(shader.program());
		gl.uniform(std::move(uniforms));
		gl.render(mesh_grid);
	}
};

struct FleetRender {
	libv::glr::Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	ShaderColor shader{shader_manager, "flat.vs", "flat.fs"};

public:
	FleetRender() {
		// TODO P1: libv.rev: Auto block binding based on include detection | intermediate step: integrate to shader function type / override
		shader.program().block_binding(uniformBlock_matrices);

		draw_mesh(mesh);
	}

	void draw_mesh(libv::glr::Mesh& mesh) {
		auto position = mesh.attribute(attribute_position);
		auto normal = mesh.attribute(attribute_normal);
		auto texture0 = mesh.attribute(attribute_texture0);
		auto index = mesh.index();

		libv::glr::generateSpherifiedCube(8, position, normal, texture0, index);
//		libv::glr::generateCube(position, normal, texture0, index);
	}

	void render(libv::glr::Queue& gl, libv::glr::UniformBuffer& uniform_stream) {
		auto uniforms = uniform_stream.block_unique(layout_matrices);
		uniforms[layout_matrices.matMVP] = gl.mvp();
		uniforms[layout_matrices.matM] = gl.model;
		uniforms[layout_matrices.matP] = gl.projection;
		uniforms[layout_matrices.eye] = gl.eye();

		gl.program(shader.program());
		gl.uniform(shader.uniform().base_color, libv::vec4f(0.7f, 0.7f, 0.7f, 1.0f));
		gl.uniform(std::move(uniforms));
		gl.render(mesh);
	}
};

//struct Fleet {
//	enum class CommandType {
//		movement,
//		attack,
////		follow,
////		merge,
////		block,
////		land,
////		...,
//	};
//
//	struct Command {
//		CommandType type;
//		libv::vec3f target;
////		int32_t target;
//	};
//
//public:
//	libv::vec3f position;
//	libv::vec3f movement;
//	CommandArrow command_arrow; // <<< Yes, but no, maybe-, Think about it how should ownership and references fork regarding renderers
//	std::vector<Command> commands;
//
//public:
//	void queue_command(CommandType type, libv::vec3f target) {
//		commands.emplace_back(type, target);
////		command_arrow.add(target, color);
//	}
////
////	void queue_command(CommandType type, int32_t target) {
////
////	}
//};

//struct Map {
//	FleetRender fleet_render;
//	std::vector<Fleet> fleets;
//
//	void render_fleets(libv::glr::Queue& gl, libv::vec2f canvas_size, libv::glr::UniformBuffer& uniform_stream) {
//		for (const auto& fleet : fleets) {
//			const auto m_guard = gl.model.push_guard();
//			gl.model.translate(fleet.position);
//			fleet_render.render(gl, uniform_stream);
//		}
//	}
//
//	void render_fleet_arrows(libv::glr::Queue& gl, libv::vec2f canvas_size, libv::glr::UniformBuffer& uniform_stream) {
//		for (const auto& fleet : fleets)
//			fleet.command_arrow.render(gl, canvas_size, uniform_stream);
//	}
//};

// -------------------------------------------------------------------------------------------------

libv::vec3f intersect_ray_plane(libv::vec3f ray_point, libv::vec3f ray_dir, libv::vec3f plane_point, libv::vec3f plane_normal) {
	const auto diff = ray_point - plane_point;
	const auto prod1 = libv::vec::dot(diff, plane_normal);
	const auto prod2 = libv::vec::dot(ray_dir, plane_normal);
	const auto prod3 = prod1 / prod2;
	return ray_point - ray_dir * prod3;
}

// -------------------------------------------------------------------------------------------------

struct SpaceCanvas : libv::ui::Canvas {
	app::CameraPlayer& camera;
	app::CameraPlayer::screen_picker screen_picker;

	float angle = 0.0f;
	float time = 0.0f;
	float test_sin_time = 0.0f;

	Background background;
	Grid grid;
	Gizmo origin_gizmo;
	CommandArrow arrow;
	FleetRender render_fleet;
	std::vector<libv::vec3f> fleet_positions;

	libv::glr::UniformBuffer uniform_stream{libv::gl::BufferUsage::StreamDraw};

	explicit SpaceCanvas(app::CameraPlayer& camera) :
		camera(camera),
		screen_picker(camera.picker({100, 100})) { // <<< This line is wrong, canvas_size is not initialized at this point
	}

	static void register_controls(libv::ctrl::FeatureRegister controls) {
//		libv::ctrl::scale_group sg_translate{
//				.impulse = 0.1,
//				.time = 1.0,
//				.mouse = 1.0 / 600.0,
//				.scroll = 0.1,
//				.gp_analog = 1.0,
//				.js_analog = 1.0
//		};

		controls.feature_action<SpaceCanvas>("space.add_fleet", [](const auto& arg, SpaceCanvas& ctx) {
			(void) arg;

			// <<< Use correct mouse coords
			const auto mouse_coord = libv::vec2f{ctx.canvas_size * 0.5f};
			const auto world_coord = intersect_ray_plane(ctx.camera.eye(), ctx.screen_picker.to_world(mouse_coord), libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 1));

			ctx.fleet_positions.emplace_back(world_coord);
			std::cout << "world_coord: " << world_coord << std::endl;
		});
	}

	static void bind_default_controls(libv::ctrl::Controls& controls) {
		controls.bind("space.add_fleet", "Ctrl + LMB [press]");
	}

	virtual void update(libv::ui::time_duration delta_time) override {
		const auto dtf = static_cast<float>(delta_time.count());
		angle = std::fmod(angle + 5.0f * dtf, 360.0f);
		time += dtf;
		global_time += dtf;

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

		if (global_test_mode != 0) {
			test_sin_time += dtf;
			auto t = (std::sin(test_sin_time / 10.f) * 0.5f + 0.5f);
			if (global_test_mode == 1) {
				camera.pitch(-t * libv::pi_f * 0.5f);
			} else if (global_test_mode == 2) {
				t = t > 0.5f ? 1.f - t : t;
				camera.pitch(-t * libv::pi_f * 0.5f);
			} else if (global_test_mode == 3) {
				const float part = 4;
				auto t = (std::sin(test_sin_time / 10.f * part) * 0.5f + 0.5f);
				t = t > 0.5f ? 1.f - t : t;
				camera.pitch(-t * libv::pi_f * 0.5f / part);
			}
		}

		// <<< Events should update this (or mark as dirty), not update
		screen_picker = camera.picker(canvas_size);
	}

	virtual void render(libv::glr::Queue& gl) override {
		const auto s_guard = gl.state.push_guard();

		gl.state.enableDepthTest();
		gl.state.enableDepthMask();

		gl.state.enableBlend();
		gl.state.blendSrc_SourceAlpha();
		gl.state.blendDst_One_Minus_SourceAlpha();

		gl.state.cullBackFace();
		gl.state.enableCullFace();

//		gl.state.polygonModeFill();

		gl.projection = camera.projection(canvas_size);
		gl.view = camera.view();
		gl.model = libv::mat4f::identity();

//		gl.setClearColor(0, 0, 0, 0); // For debug, this would break some of the UI
//		gl.clearColor();

		// --- Render Opaque ---

		for (const auto& position : fleet_positions) {
			const auto m_guard = gl.model.push_guard();
			gl.model.translate(position);
			gl.model.scale(0.2f);
			render_fleet.render(gl, uniform_stream);
//			origin_gizmo.render(gl, uniform_stream);
		}

		// --- Render Background/Sky ---

		{
			const auto s_guard2 = gl.state.push_guard();
			// No need to write depth data for the background
			gl.state.disableDepthMask();
			background.render(gl, canvas_size);
		}

		// --- Render Transparent ---

		arrow.render(gl, canvas_size, uniform_stream);

		// --- Render UI/HUD ---

		{
			const auto s_guard2 = gl.state.push_guard();
			gl.state.disableDepthMask();

			{ // Grid
				grid.render(gl, uniform_stream);
			}

			{ // Camera orbit point
				const auto m_guard = gl.model.push_guard();
				gl.model.translate(camera.orbit_point());
				gl.model.scale(0.2f);
				origin_gizmo.render(gl, uniform_stream);
			}

			gl.state.disableDepthTest();

			{ // Camera orientation gizmo in top right
				const auto p_guard = gl.projection.push_guard();
				const auto v_guard = gl.view.push_guard();
				const auto m_guard = gl.model.push_guard();

				const auto orientation_gizmo_size = 64.f; // The axes of the gizmo will be half of this size
				const auto orientation_gizmo_margin = 4.f;

				gl.projection = libv::mat4f::ortho(
						-canvas_size + orientation_gizmo_size * 0.5f + orientation_gizmo_margin,
						canvas_size,
						-orientation_gizmo_size,
						+orientation_gizmo_size);
				gl.view = camera.orientation().translate(-1, 0, 0);
				gl.model.scale(orientation_gizmo_size * 0.5f);

				origin_gizmo.render(gl, uniform_stream);
			}
		}
	}
};

// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================

//namespace libv {
//namespace ui {
//
//// -------------------------------------------------------------------------------------------------
//
//class StatusLog {
//private:
//	using EntryID = std::string;
//
//	struct LogEntry {
//		std::string id;
//		std::string style;
//		libv::ui::Label label;
//		time_point death_time;
////		time_duration lifetime;
//	};
//
//private:
//	std::vector<LogEntry> entries;
//	std::unordered_map<EntryID, std::shared_ptr<LogEntry>> index;
//
////public:
////	void add(EntryID id, std::string style, std::string text, time_duration lifetime);
////	void remove(EntryID id);
////
////private:
////	virtual void update(time_duration dt) override {
////
////	}
//};
//
//// -------------------------------------------------------------------------------------------------
//
//} // namespace ui
//} // namespace libv

// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------

// TODO P1: Controls camera should only be placed into context if the canvas is focused | Remove the F12 tracking manual workaround too for mode switching (this might be a second task, as it could be two separate system)
// TODO P2: UI Canvas, find a better API, let the component take the ownership of the canvas
// TODO P3: Arrow strip control from lua (or something lua related) (With auto reload and everything)
// TODO P4: Nicer arrow strips, animation


int main() {
	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");

	std::cout << libv::logger_stream;
	log_space.info("Hello Space!");

	// Change working directory
	std::filesystem::current_path("app/space/");

	libv::Frame frame("Space", 1280, 800);
	frame.setPosition(libv::Frame::FramePosition::center_current_monitor);
	frame.setOpenGLProfile(libv::Frame::OpenGLProfile::core);
	frame.setOpenGLVersion(3, 3);
	frame.setOpenGLSamples(libv::Frame::OpenGLSamples{4});
	frame.setOpenGLRefreshRate(libv::Frame::OpenGLRefreshRate{1});
	frame.setIcon(app::icon_set_iris_cyan());

	libv::ui::Settings ui_settings;
	// TODO P1: Internalize used UI resources under space, currently: app/space/../../res/
	ui_settings.res_font.base_path = "../../res/font/";
	ui_settings.res_shader.base_path = "../../res/shader/";
	ui_settings.res_texture.base_path = "../../res/texture/";
	libv::ui::UI ui(ui_settings);

	libv::ctrl::Controls controls;

	app::CameraBehaviour::register_controls(controls);
	app::CameraBehaviour::bind_default_controls(controls);
	SpaceCanvas::register_controls(controls);
	SpaceCanvas::bind_default_controls(controls);

	app::CameraPlayer camera;
	camera.look_at({1.6f, 1.6f, 1.2f}, {0.5f, 0.5f, 0.f});
	controls.context_enter<app::BaseCameraOrbit>(&camera); // TODO P4: <app::BaseCameraOrbit> Question mark? Context variables and inheritance?

	libv::Timer timer;
	frame.onContextUpdate.output([&](const auto&) {
		// shader_manager.update MUST run before any other render queue operation
		// OTHERWISE the not loaded uniform locations are attempted to be used and placed into the streams
		shader_manager.update(ui.gl());

		// TODO P1: Timer management should be smoother with control and frame attachment -> controls.attach(frame)
		controls.update(timer.time());
	});

	ui.attach(frame);
	controls.attach(frame);

	SpaceCanvas space(camera);
	controls.context_enter<SpaceCanvas>(&space); // TODO P1: Enter / leave on canvas focus-unfocus

	frame.onKey.output([&](const libv::input::EventKey& e) {
		if (e.keycode == libv::input::Keycode::Escape)
			frame.closeForce();

		// TODO P1: Ui focus base camera context switch
		// Hack workaround until ui focus does not operates control contexts
		if (e.keycode == libv::input::Keycode::F12 && e.action == libv::input::Action::press) {
			static int hack_camera_control_ui_mode = 0;
			hack_camera_control_ui_mode = (hack_camera_control_ui_mode + 1) % 3;
			if (hack_camera_control_ui_mode == 1)
				controls.context_leave<app::BaseCameraOrbit>();
			else
				controls.context_enter<app::BaseCameraOrbit>(&camera);
		}

		// TODO P1: Shortcut to save camera position and reload it upon restart
		//          > Requires persistence
		// TODO P1: Persist auto runtime hook options
		//          > Requires persistence
		// TODO P1: Auto runtime hook option for random uniform variables
		if (e.keycode == libv::input::Keycode::C && e.action != libv::input::Action::release) {
			const int32_t mode_count = 4;
			if (frame.isKeyPressed(libv::input::Keycode::ShiftLeft) || frame.isKeyPressed(libv::input::Keycode::ShiftRight))
				global_test_mode = global_test_mode == 0 ? mode_count - 1 : global_test_mode - 1;
			else
				global_test_mode = (global_test_mode + 1) % mode_count;
		}
		if (e.keycode == libv::input::Keycode::Backtick)
			global_test_mode = 0;
		if (e.keycode == libv::input::Keycode::Num1)
			global_test_mode = 1;
		if (e.keycode == libv::input::Keycode::Num2)
			global_test_mode = 2;
		if (e.keycode == libv::input::Keycode::Num3)
			global_test_mode = 3;
		log_space.info("Test mode: {}", global_test_mode);
	});

//	std::cout << libv::glr::layout_to_string<UniformLayoutMatrices>("Sphere") << std::endl;

	std::map<libv::rev::ShaderID, std::string> shader_states; // TODO P1: Shove it into status_log

	{
		libv::ui::PanelFull layers;
		libv::ui::Label label;
		label.align_horizontal(libv::ui::AlignHorizontal::left);
		label.align_vertical(libv::ui::AlignVertical::bottom);
//		label.align_horizontal(libv::ui::AlignHorizontal::center);
//		label.align_vertical(libv::ui::AlignVertical::center);
		label.text("");
//		label.font_color({.7f, 0.2f, 0.4f, 1.f});
//		label.font_color({.95f, 0.25f, 0.35f, 1.f});
		label.font_color({.9333f, 0.8235f, 0.0078f, 1.f}); // Warning yellow

//		libv::ui::StatusLog status_log;
//		status_log.align_horizontal(libv::ui::AlignHorizontal::center);
//		status_log.align_vertical(libv::ui::AlignVertical::center);
//		status_log.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);

		// TODO P1: [label] causes lifetimes issues, ui event hub or weak_ptr like behaviour is required
		shader_manager.on_update([label, &shader_states](const libv::rev::ShaderLoadSuccess& e) mutable {
			shader_states.erase(e.id);

			label.text("");
			for (const auto& shader_state : shader_states)
				label.text(label.text() + "\n" + shader_state.second);
		});
		shader_manager.on_update([label, &shader_states](const libv::rev::ShaderLoadFailure& e) mutable {
			if (e.include_failure) {
				auto& message = shader_states[e.id];
				message += fmt::format("--- Failed to load shader: {} ({}) ---\n", e.shader.name(), e.id);
				message += fmt::format("Failed to include: \"{}\" from file: \"{}\" - {}: {}", e.include_failure->include_path, e.include_failure->file_path, e.include_failure->ec, e.include_failure->ec.message());
				for (const auto& [file, line] : e.include_failure->include_stack)
					message += fmt::format("\n    Included from: {}:{}", file, line);

			} else if (e.compile_failure) {
				shader_states[e.id] = fmt::format("--- Failed to compile shader: {} ({}) ---\n{}", e.shader.name(), e.id, e.compile_failure->message);

			} else if (e.link_failure) {
				shader_states[e.id] = fmt::format("--- Failed to link shader: {} ({}) ---\n{}", e.shader.name(), e.id, e.link_failure->message);
			}

			label.text("");
			for (const auto& shader_state : shader_states)
				label.text(label.text() + "\n" + shader_state.second);
		});
		shader_manager.on_update([label, &shader_states](const libv::rev::ShaderUnload& e) mutable {
			shader_states.erase(e.id);

			label.text("");
			for (const auto& shader_state : shader_states)
				label.text(label.text() + "\n" + shader_state.second);
		});

		// ---
		// TODO P4: libv.ui: Implement UI based event routing hub
		// TODO P4: space: Use UI based event routing hub instead of lambda connection
		// TODO P1: Hook in shader loader update event callback
//		shader_manager.on_update([status_log](const libv::rev::ShaderLoader::Update& update) {
//			status_log.add(
//					update.id,
//					update.success ? "shader_success" : "shader_failure",
//					update.message(),
//					update.success ? std::chrono::seconds(1) : 0);
//		});
		// ---

		libv::ui::CanvasAdaptor canvas;
		canvas.adopt(&space);
//		libv::ui::Canvas2<SpaceCanvas> canvas;

		layers.add(label);
		layers.add(canvas);
//		layers.add(status_log);
//		layers.add(pref_graph);
		ui.add(layers);
	}

	frame.show();
	frame.join();

	shader_manager.clear_on_updates(); // TODO P1: [label] forces this one where, solve that event-ui-lifetime issue

	return EXIT_SUCCESS;
}
