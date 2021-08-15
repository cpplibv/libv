// Project: libv, File: app/space/render.cpp, Author: Császár Mátyás [Vader]

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
	//	std::vector<libv::vec3f> points{{0, 0, 0}, {1, 0.5, 0.5}, {1, 1, 0}, {1, 2, 2}, {-1, -1, -1}};
	//	for (int i = 0; i < 60; i++) {
	//		const auto r = i / 30.0;
	//		const auto x = std::sin(libv::deg_to_rad(i * 15.0)) * r;
	//		const auto y = std::cos(libv::deg_to_rad(i * 15.0)) * r;
	//		points.emplace_back(x, y, 0);
	//	}
	//	for (int i = 60; i < 120; i++) {
	//		const auto r = 2.0 - (i - 60) / 30.0 * 0.5;
	//		const auto x = std::sin(libv::deg_to_rad(i * 15.0)) * r;
	//		const auto y = std::cos(libv::deg_to_rad(i * 15.0)) * r;
	//		const auto z = std::sin(libv::deg_to_rad((i - 60) * 30.0)) * 0.25;
	//		points.emplace_back(x, y, z);
	//	}
	//
	//	build_mesh(mesh, points);
}

void RendererCommandArrow::add_arrow(libv::vec3f source, libv::vec3f target, ArrowStyle style) {
	if (source == target) // Sanity check
		return;

	arrows.emplace_back(source, target, style);
}

void RendererCommandArrow::rebuild_mesh() {
	mesh.clear(); // <<< Better support for Dynamic VAO data

	auto position = mesh.attribute(attribute_position);
	auto color0 = mesh.attribute(attribute_color0);
	auto sp_ss_tp_ts = mesh.attribute(attribute_custom0); // SegmentPosition, SegmentSize, TotalPosition, TotalSize
	auto index = mesh.index();

	libv::glr::VertexIndex i = 0;
	for (const auto& arrow : arrows) {
		const auto length = (arrow.target - arrow.source).length();

		position(arrow.source);
		position(arrow.target);

		color0(arrow.style.color_source);
		color0(arrow.style.color_target);

		sp_ss_tp_ts(0, length, 0, length);
		sp_ss_tp_ts(length, length, length, length);

		index.line(i + 0, i + 1);
		i += 2;
	}

	arrows.clear();

	//	float total_length = 0;
	//	float current_length = 0;
	//
	//	libv::algo::adjacent_pairs(points, [&](auto a, auto b) {
	//		total_length += (b - a).length();
	//	});

	//	libv::algo::adjacent_pairs(points, [&](auto a, auto b) {
	//		if (a == b) // Sanity check
	//			return;
	//
	//		const auto length = (b - a).length();
	//
	//		position(a);
	//		position(b);
	//
	//		color0(1, 1, 1, 1);
	//		color0(1, 1, 1, 1);
	//
	//		sp_ss_tp_ts(0, length, current_length, total_length);
	//		sp_ss_tp_ts(length, length, current_length, total_length);
	//
	//		index.line(i + 0, i + 1);
	//		i += 2;
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
