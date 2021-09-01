// Project: libv, File: app/space/renderer.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <space/renderer.hpp>
// libv
#include <libv/ui/ui.hpp>
#include <libv/ui/event_hub.hpp>
//#include <libv/algo/adjacent_pairs.hpp>
//#include <libv/ctrl/controls.hpp>
//#include <libv/ctrl/feature_register.hpp>
#include <libv/glr/procedural/sphere.hpp>
//#include <libv/glr/program.hpp>
#include <libv/glr/queue.hpp>
#include <libv/math/noise/white.hpp>
//#include <libv/glr/texture.hpp>
// pro
//#include <space/camera.hpp>
//#include <space/command.hpp>
//#include <space/game_instance.hpp>
//#include <space/playout.hpp>
//#include <space/universe/universe.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

RendererEditorBackground::RendererEditorBackground(RendererResourceContext& rctx) :
		shader(rctx.shader_manager, "editor_background.vs", "editor_background.fs") {

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
		position(1, -1, 1);
		position(1, 1, 1);
		position(-1, 1, 1);

		index.quad(0, 1, 2, 3);
	}
}

void RendererEditorBackground::render(libv::glr::Queue& gl, libv::vec2f canvas_size) {
	gl.program(shader.program());
	const auto bg_noise = libv::vec4f(1, 1, 1, 0) * (5.f / 255.f);
	const auto bg_color = libv::vec4f(0.098f, 0.2f, 0.298f, 1.0f);
	gl.uniform(shader.uniform().render_resolution, canvas_size);
	gl.uniform(shader.uniform().noise_scale, bg_noise);
	gl.uniform(shader.uniform().base_color, bg_color);
	gl.texture(background_texture_pattern, textureChannel_pattern);
	gl.render(mesh_background);
}

// -------------------------------------------------------------------------------------------------

RendererCommandArrow::RendererCommandArrow(RendererResourceContext& rctx) :
		shader{rctx.shader_manager, "command_arrow.vs", "command_arrow.gs", "command_arrow.fs"} {
}

void RendererCommandArrow::restart_chain(float animation_offset) {
//	if(!arrows.empty()){
//		arrows.back().start_of_chain = true;
//	}
	curr_animation_offset = animation_offset;
	curr_start_of_chain = true;
}

void RendererCommandArrow::add_arrow(libv::vec3f source, libv::vec3f target, ArrowStyle style) {
	if (source == target) // Sanity check
		return;

	arrows.emplace_back(source, target, curr_animation_offset, style, std::exchange(curr_start_of_chain, false));
}

void RendererCommandArrow::add_debug_spiral() {
	restart_chain(0);

	add_arrow({0, 0, 0}, {1, 0.5f, 0.5f}, debug_arrow_style);
	add_arrow({1, 0.5f, 0.5f}, {1, 1, 0}, debug_arrow_style);
	add_arrow({1, 1, 0}, {1, 2, 2}, debug_arrow_style);
	add_arrow({1, 2, 2}, {-1, -1, -1}, debug_arrow_style);

	libv::vec3f prev_point;
	for (int i = 0; i < 60; i++) {
		const auto if_ = static_cast<float>(i);

		const auto r = if_ / 30.f;
		const auto x = std::sin(libv::deg_to_rad(if_ * 15.f)) * r;
		const auto y = std::cos(libv::deg_to_rad(if_ * 15.f)) * r;

		const auto curr_point = libv::vec3f{x, y, 0};
		add_arrow(prev_point, curr_point, debug_arrow_style);
		prev_point = curr_point;
	}
	for (int i = 60; i < 120; i++) {
		const auto if_ = static_cast<float>(i);

		const auto r = 2.f - (if_ - 60.f) / 30.f * 0.5f;
		const auto x = std::sin(libv::deg_to_rad(if_ * 15.f)) * r;
		const auto y = std::cos(libv::deg_to_rad(if_ * 15.f)) * r;
		const auto z = std::sin(libv::deg_to_rad((if_ - 60.f) * 30.f)) * 0.25f;

		const auto curr_point = libv::vec3f{x, y, z};
		add_arrow(prev_point, curr_point, debug_arrow_style);
		prev_point = curr_point;
	}
}

void RendererCommandArrow::add_debug_view01() {
	restart_chain(0);
	for (int i = 0; i < 10; i++) {
		const auto if_ = static_cast<float>(i);

		add_arrow({5, std::pow(2.f, if_) - 1, 0}, {5, std::pow(2.f, if_ + 1) - 1, 0}, debug_arrow_style);
	}
}

void RendererCommandArrow::add_debug_view02() {
	const auto d = 0.5f;
	const auto max_n = 30;

	for (int i = 3; i <= max_n; ++i) {
		const auto if_ = static_cast<float>(i);

		restart_chain(0);
		for (int j = 0; j < i; ++j) {
			const auto jf = static_cast<float>(j);

			const auto x1 = std::sin(libv::tau / if_ * jf) * d;
			const auto y1 = std::cos(libv::tau / if_ * jf) * d;
			const auto x2 = std::sin(libv::tau / if_ * (jf - 1)) * d;
			const auto y2 = std::cos(libv::tau / if_ * (jf - 1)) * d;
			add_arrow({x1 + 3, if_ - 3, y1}, {x2 + 3, if_ - 3, y2}, debug_arrow_style);
		}
	}
}

void RendererCommandArrow::add_debug_view03() {
	const auto d = 0.5f;
	const auto max_n = 30;

	for (int i = 3; i <= max_n; ++i) {
		const auto if_ = static_cast<float>(i);

		restart_chain(0);
		for (int j = 0; j < i; ++j) {
			const auto jf = static_cast<float>(j);

			const auto x1 = std::sin(libv::tau / if_ * jf) * d;
			const auto y1 = std::cos(libv::tau / if_ * jf) * d;
			add_arrow({x1 + 6, if_ - 3, y1}, {6, if_ - 3, 0}, debug_arrow_style);
		}
	}
}

void RendererCommandArrow::add_debug_view04() {
	const auto d = 0.5f;
	const auto max_n = 30;

	for (int i = 3; i <= max_n; ++i) {
		const auto if_ = static_cast<float>(i);

		restart_chain(0);
		for (int j = 0; j < i; ++j) {
			const auto jf = static_cast<float>(j);

			const auto x1 = std::sin(libv::tau / if_ * jf) * d;
			const auto y1 = std::cos(libv::tau / if_ * jf) * d;
			add_arrow({9, if_ - 3, 0}, {x1 + 9, if_ - 3, y1}, debug_arrow_style);
		}
	}
}

void RendererCommandArrow::add_debug_view05() {
	for (int i = 0; i < 100; i++) {
		const auto if_ = static_cast<float>(i);

		restart_chain(0);
		add_arrow({10, if_, 0}, {10 + (if_ + 1) * 0.25f, if_, 0}, debug_arrow_style);
	}
}

void RendererCommandArrow::rebuild_mesh() {
	mesh.clear(); // <<< Better support for Dynamic VAO data

	auto position = mesh.attribute(attribute_position);
	auto color0 = mesh.attribute(attribute_color0);
	auto sp_ss_cp_cs = mesh.attribute(attribute_custom0); // SegmentPosition, SegmentSize, ChainPosition, ChainSize
	auto ao_nu_nu_nu = mesh.attribute(attribute_custom1); // AnimationOffset, NotUsed, NotUsed, NotUsed
	auto index = mesh.index();

	libv::glr::VertexIndex i = 0;

	auto chain_pos = 0.f;
	auto chain_size = 0.f;

	for (size_t j = 0; j < arrows.size(); ++j) {
		const auto& arrow = arrows[j];
		const auto length = (arrow.target - arrow.source).length();

		if (arrow.start_of_chain == true) {
			// TODO P5: Instead of forward scan on rebuild mesh, calculate and assign values on add_arrow() and on start_chain()
			chain_pos = 0.f;
			chain_size = length;
			for (size_t k = j + 1; k < arrows.size() && !arrows[k].start_of_chain; ++k) {
				const auto& a = arrows[k];
				chain_size += (a.target - a.source).length();
			}
		}

		position(arrow.source);
		position(arrow.target);

		color0(arrow.style.color_source);
		color0(arrow.style.color_target);

		ao_nu_nu_nu(arrow.animation_offset, 0, 0, 0);
		ao_nu_nu_nu(arrow.animation_offset, 0, 0, 0);

		sp_ss_cp_cs(0, length, chain_pos, chain_size);
		sp_ss_cp_cs(length, length, chain_pos + length, chain_size);

		index.line(i + 0, i + 1);
		i += 2;

		chain_pos += length;
	}

	arrows.clear();

	//	float chain_length = 0;
	//	float current_length = 0;
	//
	//	libv::algo::adjacent_pairs(points, [&](auto a, auto b) {
	//		chain_length += (b - a).length();
	//	});
	//
	//	libv::algo::adjacent_pairs(points, [&](auto a, auto b) {
	//		const auto length = (b - a).length();
	//
	//		sp_ss_tp_ts(0, length, current_length, chain_length);
	//		sp_ss_tp_ts(length, length, current_length, chain_length);
	//
	//		current_length += length;
	//	});
}

void RendererCommandArrow::render(libv::glr::Queue& gl, libv::vec2f canvas_size, libv::glr::UniformBuffer& uniform_stream) {
	rebuild_mesh();

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

	gl.program(shader.program());
	gl.uniform(std::move(uniforms));
	gl.uniform(shader.uniform().color, libv::vec4f(1.0f, 1.0f, 1.0f, 1.0f));
	gl.uniform(shader.uniform().render_resolution, canvas_size);
	gl.uniform(shader.uniform().test_mode, global_test_mode);
	gl.uniform(shader.uniform().time, global_time);
	gl.render(mesh);
}

// -------------------------------------------------------------------------------------------------

RendererGizmo::RendererGizmo(RendererResourceContext& rctx) :
		shader(rctx.shader_manager, "editor_gizmo.vs", "editor_gizmo.fs") {
	build_gizmo_lines(mesh);
}

void RendererGizmo::build_gizmo_lines(libv::glr::Mesh& mesh) {
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

void RendererGizmo::render(libv::glr::Queue& gl, libv::glr::UniformBuffer& uniform_stream) {
	auto uniforms = uniform_stream.block_unique(layout_matrices);
	uniforms[layout_matrices.matMVP] = gl.mvp();
	uniforms[layout_matrices.matM] = gl.model;
	uniforms[layout_matrices.matP] = gl.projection;
	uniforms[layout_matrices.eye] = gl.eye();

	gl.program(shader.program());
	gl.uniform(std::move(uniforms));
	gl.render(mesh);
}

// -------------------------------------------------------------------------------------------------

RendererEditorGrid::RendererEditorGrid(RendererResourceContext& rctx) :
		shader(rctx.shader_manager, "editor_grid_plane.vs", "editor_grid_plane.fs") {

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

void RendererEditorGrid::render(libv::glr::Queue& gl, libv::glr::UniformBuffer& uniform_stream) {
	auto uniforms = uniform_stream.block_unique(layout_matrices);
	uniforms[layout_matrices.matMVP] = gl.mvp();
	uniforms[layout_matrices.matM] = gl.model;
	uniforms[layout_matrices.matP] = gl.projection;
	uniforms[layout_matrices.eye] = gl.eye();

	gl.program(shader.program());
	gl.uniform(std::move(uniforms));
	gl.render(mesh_grid);
}

// -------------------------------------------------------------------------------------------------

RendererFleet::RendererFleet(RendererResourceContext& rctx) :
		shader(rctx.shader_manager, "flat.vs", "flat.fs") {
	build_mesh(mesh);
}

void RendererFleet::build_mesh(libv::glr::Mesh& mesh) {
	auto position = mesh.attribute(attribute_position);
	auto normal = mesh.attribute(attribute_normal);
	auto texture0 = mesh.attribute(attribute_texture0);
	auto index = mesh.index();

	libv::glr::generateSpherifiedCube(8, position, normal, texture0, index);
	//		libv::glr::generateCube(position, normal, texture0, index);
}

void RendererFleet::render(libv::glr::Queue& gl, libv::glr::UniformBuffer& uniform_stream) {
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

// -------------------------------------------------------------------------------------------------

Renderer::Renderer(libv::ui::UI& ui) {
	resource_context.shader_manager.attach_libv_ui_hub(ui.event_hub());
}

void Renderer::prepare_for_render(libv::glr::Queue& gl) {
	// shader_manager.update MUST run before any other render queue operation
	// OTHERWISE the not loaded uniform locations are attempted to be used and placed into the streams
	// | So this is a better place than before, still not the best, When UI gets rev updates in the future maybe there will be better solutions
	resource_context.shader_manager.update(gl.out_of_order_gl());
}

// -------------------------------------------------------------------------------------------------

} // namespace app
