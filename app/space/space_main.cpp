// Project: libv, File: app/space/space_main.cpp, Author: Császár Mátyás [Vader]

// ext
#include <fmt/chrono.h>
#include <fmt/format.h>
// libv
#include <libv/algo/adjacent_pairs.hpp>
#include <libv/ctrl/controls.hpp>
#include <libv/ctrl/feature_register.hpp>
#include <libv/frame/frame.hpp>
#include <libv/glr/attribute.hpp>
#include <libv/glr/layout_to_string.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/procedural/cube.hpp>
#include <libv/glr/procedural/sphere.hpp>
#include <libv/glr/program.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform_buffer.hpp>
#include <libv/log/log.hpp>
#include <libv/math/distance/intersect.hpp>
#include <libv/math/noise/white.hpp>
//#include <libv/math/ray.hpp>
#include <libv/rev/shader.hpp>
#include <libv/rev/shader_loader.hpp>
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/canvas.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_float.hpp>
#include <libv/ui/component/panel_full.hpp>
#include <libv/ui/component/panel_status_line.hpp>
#include <libv/ui/event_hub.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/ui/settings.hpp>
#include <libv/ui/ui.hpp>
#include <libv/utility/hex_dump.hpp>
#include <libv/utility/timer.hpp>
// std
#include <iostream>
// pro
#include <space/camera.hpp>
#include <space/camera_behaviour.hpp>
#include <space/command.hpp>
#include <space/icon_set.hpp>
#include <space/world.hpp>

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

constexpr auto attribute_position  = libv::glr::Attribute<0, libv::vec3f>{};
constexpr auto attribute_normal	= libv::glr::Attribute<1, libv::vec3f>{};
constexpr auto attribute_color0	= libv::glr::Attribute<2, libv::vec4f>{};
constexpr auto attribute_texture0 = libv::glr::Attribute<8, libv::vec2f>{};
constexpr auto attribute_custom0 = libv::glr::Attribute<15, libv::vec4f>{};

constexpr auto textureChannel_diffuse = libv::gl::TextureChannel{0};
constexpr auto textureChannel_normal  = libv::gl::TextureChannel{1};
constexpr auto textureChannel_pattern  = libv::gl::TextureChannel{7};

struct UniformLayoutMatrices {
	// Could be split into two: Camera and Model
	// 			Camera: matV, matP, near, far, ...
	// 			Model: matM, matMVP

	libv::glr::Uniform_mat4f matMVP;
	libv::glr::Uniform_mat4f matP;
	libv::glr::Uniform_mat4f matM;
	libv::glr::Uniform_vec3f eye;

	LIBV_REFLECTION_ACCESS(matMVP);
	LIBV_REFLECTION_ACCESS(matP);
	LIBV_REFLECTION_ACCESS(matM);
	LIBV_REFLECTION_ACCESS(eye);
};

//struct UniformLayoutPass {
//	libv::glr::Uniform_float time_sim;
//	libv::glr::Uniform_float time_real;
//	libv::glr::Uniform_float time_real_hcap;
//	libv::glr::Uniform_vec2f render_resolution;
//	libv::glr::Uniform_vec2f pixel_size; // 1.0 / render_resolution
//
//	libv::glr::Uniform_int32 test_mode0;
//
//    libv::glr::Uniform_vec4f frame_random_vec4; // [0..1] random numbers changing each frame
//    libv::glr::Uniform_vec4i frame_random_ivec4; // [int_min..int_max] random numbers changing each frame
//
//	LIBV_REFLECTION_ACCESS(time_sim);
//	LIBV_REFLECTION_ACCESS(time_real);
//	LIBV_REFLECTION_ACCESS(time_real_hcap);
//	LIBV_REFLECTION_ACCESS(render_resolution);
//	LIBV_REFLECTION_ACCESS(pixel_size);
//
//	LIBV_REFLECTION_ACCESS(test_mode0);
//
//	LIBV_REFLECTION_ACCESS(frame_random_vec4);
//	LIBV_REFLECTION_ACCESS(frame_random_ivec4);
//};

const auto uniformBlock_matrices = libv::glr::UniformBlockBinding{0, "Matrices"};
//const auto uniformBlock_pass = libv::glr::UniformBlockBinding{1, "Pass"};
const auto layout_matrices = libv::glr::layout_std140<UniformLayoutMatrices>(uniformBlock_matrices);
//const auto layout_pass = libv::glr::layout_std140<UniformLayoutPass>(uniformBlock_pass);

// -------------------------------------------------------------------------------------------------

libv::rev::ShaderLoader shader_manager("shader/");

// -------------------------------------------------------------------------------------------------

// TODO P2: libv.glr: Shader automated block binding by watching the includes
//          So this should work:
//          shader_loader.register_block<UniformLayoutMatrices>("block/sphere.glsl");
//          Note that this also could generate the file block/sphere.glsl (OR just be an in memory resource)
//          And this would be called on any program that includes block/sphere.glsl
//          program.block_binding(uniformBlock_matrices) and access_blocks(Access&) will be replaced by this system
//          | issue: some struct might have already been defined, so block to string might have to skip them
//                  This mean tracking of structs OR pushing the problem back to the include system pragma once solution
//                  With additional mapping and includes to struct/my_struct_that_is_in_a_block.glsl

struct UniformsBackground {
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
	template <typename Access> void access_blocks(Access&) {
	}
};

struct UniformsTestMode {
	libv::glr::Uniform_int32 test_mode;

	template <typename Access> void access_uniforms(Access& access) {
		access(test_mode, "test_mode", 0);
	}
	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
	}
};

struct UniformsColor {
	libv::glr::Uniform_vec4f base_color;

	template <typename Access> void access_uniforms(Access& access) {
		access(base_color, "base_color");
	}
	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
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
	template <typename Access> void access_blocks(Access& access) {
		access(uniformBlock_matrices);
	}
};

using ShaderBackground = libv::rev::Shader<UniformsBackground>;
using ShaderCommandArrow = libv::rev::Shader<UniformsCommandArrow>;
using ShaderColor = libv::rev::Shader<UniformsColor>;
using ShaderTestMode = libv::rev::Shader<UniformsTestMode>;
//using Shader = libv::rev::Shader<>;

// =================================================================================================

// TODO P1: kill these ones, aka implement 'pass' / 'program' uniform block (requires glr shared block allocation)
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

			position(-1, -1, 1);
			position( 1, -1, 1);
			position( 1,  1, 1);
			position(-1,  1, 1);

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

// -------------------------------------------------------------------------------------------------

struct SpaceCanvas : libv::ui::CanvasBase {
	bool main_canvas;
	app::SpaceState& state;
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

	libv::glr::UniformBuffer uniform_stream{libv::gl::BufferUsage::StreamDraw};

	explicit SpaceCanvas(app::SpaceState& state, app::CameraPlayer& camera, bool main_canvas) :
		main_canvas(main_canvas),
		state(state),
		camera(camera),
		screen_picker(camera.picker({100, 100})) {
		// <<< screen_picker ctor: This line is wrong, canvas_size is not initialized at this point
		//			Component shall not receive any event before onLayout gets called
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

		controls.feature_action<SpaceCanvas>("space.add_fleet_by_mouse", [](const auto& arg, SpaceCanvas& ctx) {
			(void) arg;

			const auto mouse_local_coord = ctx.calculate_local_mouse_coord();
			const auto mouse_ray_dir = ctx.screen_picker.to_world(mouse_local_coord);
			const auto mouse_ray_pos = ctx.camera.eye();
			const auto world_coord = libv::intersect_ray_plane(mouse_ray_pos, mouse_ray_dir, libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 1));

			// TODO P1: app.space: Instead of direct apply create a command, and apply that command (in this example, this can be a direct function call into sim, but the command param is a must)
			if (!ctx.state.fleets.empty())
				ctx.state.fleets.back().target = world_coord;
			ctx.state.fleets.emplace_back(world_coord);
			std::cout << "mouse_local_coord: " << mouse_local_coord << std::endl;
			std::cout << "world_coord: " << world_coord << std::endl;
		});

		controls.feature_action<SpaceCanvas>("space.warp_camera_to_mouse", [](const auto& arg, SpaceCanvas& ctx) {
			(void) arg;

			const auto mouse_local_coord = ctx.calculate_local_mouse_coord();
			const auto mouse_ray_dir = ctx.screen_picker.to_world(mouse_local_coord);
			const auto mouse_ray_pos = ctx.camera.eye();
			const auto world_coord = libv::intersect_ray_plane(mouse_ray_pos, mouse_ray_dir, libv::vec3f(0, 0, 0), libv::vec3f(0, 0, 1));

			// TODO P1: app.space: Instead of direct apply create a command, and apply that command (in this example, this can be a direct function call into sim, but the command param is a must)
			//  		(even tho its not a sim command, for replays client view tracking is also important, but it may be a different type of command)
			//			khm-khm: MVC - Model View Control
			ctx.camera.warp_to(world_coord);
			std::cout << "world_coord: " << world_coord << std::endl;
		});
	}

	static void bind_default_controls(libv::ctrl::Controls& controls) {
		controls.bind("space.add_fleet_by_mouse", "Ctrl + LMB [press]");
		controls.bind("space.warp_camera_to_mouse", "Z");
	}

	virtual void update(libv::ui::time_duration delta_time) override {
		if (main_canvas)
			state.update(delta_time);

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
	}

	virtual void render(libv::glr::Queue& gl) override {
		// NOTE: Screen_picker update has to be placed around render, as canvas_size is only set after layout
		screen_picker = camera.picker(canvas_size);
		//

		const auto s_guard = gl.state.push_guard();

		gl.state.enableDepthTest();
		gl.state.depthFunctionLess();
		gl.state.enableDepthMask();

		gl.state.enableBlend();
		gl.state.blendSrc_SourceAlpha();
		gl.state.blendDst_One_Minus_SourceAlpha();

		gl.state.cullBackFace();
		gl.state.enableCullFace();
		gl.state.frontFaceCCW();

		gl.state.clipPlanes(0);
		gl.state.polygonModeFill();

		gl.projection = camera.projection(canvas_size);
		gl.view = camera.view();
		gl.model = libv::mat4f::identity();

		// --- Render Background/Sky ---

		if (!main_canvas) {
			const auto s2_guard = gl.state.push_guard();
			// Clear the depth data for the background of the mini display
			gl.state.depthFunctionAlways();
			background.render(gl, canvas_size);
		}

		// --- Render Opaque ---

		for (const auto& fleet : state.fleets) {
			const auto m_guard = gl.model.push_guard();
			gl.model.translate(fleet.position);
			gl.model.scale(0.2f);
			render_fleet.render(gl, uniform_stream);
		}

		// --- Render Background/Sky ---

		if (main_canvas) {
			const auto s2_guard = gl.state.push_guard();
			// No need to write depth data for the main background
			gl.state.disableDepthMask();
			background.render(gl, canvas_size);
		}

		// --- Render Transparent ---

		arrow.render(gl, canvas_size, uniform_stream);

		// --- Render UI/HUD ---

		{
			{ // Grid
				const auto s2_guard = gl.state.push_guard();
				gl.state.disableDepthMask();

				grid.render(gl, uniform_stream);
			}

			{ // Camera orbit point
				const auto s2_guard = gl.state.push_guard();
				gl.state.disableDepthMask();

				const auto m_guard = gl.model.push_guard();
				gl.model.translate(camera.orbit_point());
				gl.model.scale(0.2f);
				origin_gizmo.render(gl, uniform_stream);
			}

			{ // Camera orientation gizmo in top right
				const auto s2_guard = gl.state.push_guard();
				gl.state.disableDepthTest();
				gl.state.disableDepthMask();

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

// -------------------------------------------------------------------------------------------------

// TODO P1: Controls camera should only be placed into context if the canvas is focused
// TODO P1: Remove the F12 tracking manual workaround too for mode switching (Related to controls bypass)
// TODO P3: Arrow strip placement control from lua (or something lua related) (With auto reload and everything)

struct SpaceFrame : public libv::Frame {
	libv::ui::UI ui;
	libv::ctrl::Controls controls;
	libv::Timer timer; // <<< use frame loop to call controls

	SpaceFrame() :
		Frame("Space", 1280, 800),
		ui([] {
			libv::ui::Settings ui_settings;
			// TODO P1: Internalize used UI resources under space, currently: app/space/../../res/
			ui_settings.res_font.base_path = "../../res/font/";
			ui_settings.res_shader.base_path = "../../res/shader/";
			ui_settings.res_texture.base_path = "../../res/texture/";
			return ui_settings;
		}()) {

		setPosition(libv::Frame::FramePosition::center_current_monitor);
		setOpenGLProfile(libv::Frame::OpenGLProfile::core);
		setOpenGLVersion(3, 3);
		setOpenGLSamples(libv::Frame::OpenGLSamples{4});
		setOpenGLRefreshRate(libv::Frame::OpenGLRefreshRate{1});
		setIcon(app::icon_set_iris_cyan());

		app::CameraBehaviour::register_controls(controls);
		app::CameraBehaviour::bind_default_controls(controls);
		SpaceCanvas::register_controls(controls);
		SpaceCanvas::bind_default_controls(controls);

		shader_manager.attach_libv_ui_hub(ui.event_hub());
		onContextUpdate.output([&](const auto&) {
			// shader_manager.update MUST run before any other render queue operation
			// OTHERWISE the not loaded uniform locations are attempted to be used and placed into the streams
			shader_manager.update(ui.gl());

			// TODO P1: Timer management should be smoother with control and frame attachment -> controls.attach(frame)
			controls.update(timer.time());
		});

		ui.attach(*this);
		controls.attach(*this);
	}
};

int main() {
	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");

	std::cout << libv::logger_stream;
	log_space.info("Hello Space!");

	// Change working directory
	std::filesystem::current_path("app/space/");

	SpaceFrame frame;

	app::CameraPlayer camera_main;
	app::CameraPlayer camera_mini;
	camera_main.look_at({1.6f, 1.6f, 1.2f}, {0.5f, 0.5f, 0.f});
	camera_mini.look_at({1.6f, 1.6f, 1.2f}, {0.5f, 0.5f, 0.f});

	app::SpaceState space_state;

	frame.onKey.output([&](const libv::input::EventKey& e) {
		if (e.keycode == libv::input::Keycode::Escape)
			frame.closeForce();

		// TODO P1: Ui focus base camera context switch
		// Hack workaround until ui focus does not operates control contexts
		if (e.keycode == libv::input::Keycode::F12 && e.action == libv::input::Action::press) {
			static int hack_camera_control_ui_mode = 0;
			hack_camera_control_ui_mode = (hack_camera_control_ui_mode + 1) % 3;
			if (hack_camera_control_ui_mode == 1)
				frame.controls.context_leave<app::BaseCameraOrbit>();
			else
				frame.controls.context_enter<app::BaseCameraOrbit>(&camera_main);
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

	{
//		libv::ui::PanelFull layers;
		libv::ui::PanelFloat layers;

		libv::ui::PanelStatusLine shader_errors;
		shader_errors.align_horizontal(libv::ui::AlignHorizontal::left);
		shader_errors.align_vertical(libv::ui::AlignVertical::bottom);
		shader_errors.orientation(libv::ui::Orientation::TOP_TO_BOTTOM);
//		shader_errors.size(libv::ui::parse_size_or_throw("1r, 1r"));
		shader_errors.spacing(10);

		shader_errors.event().global.connect<libv::rev::ShaderLoadSuccess>([](libv::ui::PanelStatusLine& psl, const libv::rev::ShaderLoadSuccess& e) mutable {
			if (e.shader.load_version() == 0)
				return; // Do not display the first load successes on program startup

			libv::ui::Label label;
			label.align_horizontal(libv::ui::AlignHorizontal::left);
			label.align_vertical(libv::ui::AlignVertical::bottom);
			label.size(libv::ui::parse_size_or_throw("1r, D"));
			label.font_color({.7235f, 0.9333f, 0.2433f, 1.f});

			label.text(fmt::format("[{:%H:%M:%S}] Successful shader reload: {} v{} ({})", std::chrono::system_clock::now(), e.shader.name(), e.shader.load_version(), e.id));
			psl.add(e.id, label, std::chrono::seconds(3));
		});
		shader_errors.event().global.connect<libv::rev::ShaderLoadFailure>([](libv::ui::PanelStatusLine& psl, const libv::rev::ShaderLoadFailure& e) mutable {
			libv::ui::Label label;
			label.align_horizontal(libv::ui::AlignHorizontal::left);
			label.align_vertical(libv::ui::AlignVertical::bottom);
			label.size(libv::ui::parse_size_or_throw("1r, D"));
			label.font_color({.9333f, 0.8235f, 0.0078f, 1.f}); // Warning yellow

			if (e.include_failure) {
				std::string message;
				message += fmt::format("[{:%H:%M:%S}] Failed to load shader: {} v{} ({}) using v{}\n", std::chrono::system_clock::now(), e.shader.name(), e.shader.load_version(), e.id, e.shader.current_version());
				message += fmt::format("Failed to include: \"{}\" from file: \"{}\" - {}: {}", e.include_failure->include_path, e.include_failure->file_path, e.include_failure->ec, e.include_failure->ec.message());
				for (const auto& [file, line] : e.include_failure->include_stack)
					message += fmt::format("\n    Included from: {}:{}", file, line);
				label.text(message);

			} else if (e.compile_failure) {
				label.text(fmt::format("[{:%H:%M:%S}] Failed to compile shader: {} v{} ({}) using v{}\n{}", std::chrono::system_clock::now(), e.shader.name(), e.shader.load_version(), e.id, e.shader.current_version(), e.compile_failure->message));

			} else if (e.link_failure) {
				label.text(fmt::format("[{:%H:%M:%S}] Failed to link shader: {} v{} ({}) using v{}\n{}", std::chrono::system_clock::now(), e.shader.name(), e.shader.load_version(), e.id, e.shader.current_version(), e.link_failure->message));
			}

			psl.add(e.id, label);
		});
		shader_errors.event().global.connect<libv::rev::ShaderUnload>([](libv::ui::PanelStatusLine& psl, const libv::rev::ShaderUnload& e) mutable {
			psl.remove(e.id);
		});

//		shader_manager.on_update([status_log](const libv::rev::ShaderLoader::Update& update) {
//			status_log.add(
//					update.id,
//					update.success ? "shader_success" : "shader_failure",
//					update.message(),
//					update.success ? std::chrono::seconds(1) : 0);
//		});

		// ---

		libv::ui::CanvasAdaptorT<SpaceCanvas> canvas_main("canvas-main", space_state, camera_main, true);
		libv::ui::CanvasAdaptorT<SpaceCanvas> canvas_mini("canvas-mini", space_state, camera_mini, false);
		canvas_mini.size(libv::ui::parse_size_or_throw("25%, 15%"));
		canvas_mini.padding({0, 0, 10, 0});
		canvas_mini.anchor(libv::ui::Anchor::center_right);

		libv::ui::Button clear_fleets;
		clear_fleets.align_horizontal(libv::ui::AlignHorizontal::center);
		clear_fleets.align_vertical(libv::ui::AlignVertical::center);
		clear_fleets.anchor(libv::ui::Anchor::bottom_center);
		clear_fleets.color(libv::ui::Color(0.5f, 0.5f, 0.5f, 0.65f));
		clear_fleets.padding({0, 0, 0, 0});
		clear_fleets.size(libv::ui::parse_size_or_throw("10pxD, 4pxD"));
		clear_fleets.text("Clear Fleets");
//		clear_fleets.event().submit.connect(canvas_main, [](libv::ui::CanvasAdaptorT<SpaceCanvas>& canvas) {
		clear_fleets.event().submit.connect([canvas = canvas_main]() {
			canvas.object().state.fleets.clear();
		});

		layers.add(canvas_main);
		layers.add(canvas_mini);
		layers.add(shader_errors);
		layers.add(clear_fleets);
//		layers.add(pref_graph);
		frame.ui.add(layers);

		frame.controls.context_enter<app::BaseCameraOrbit>(&camera_main); // TODO P4: <app::BaseCameraOrbit> Question mark? Context variables and inheritance?
		frame.controls.context_enter<SpaceCanvas>(&canvas_main.object()); // TODO P1: Enter / leave on canvas focus-unfocus
	}

	frame.show();
	frame.join();

	return EXIT_SUCCESS;
}
