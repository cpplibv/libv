// Project: libv, File: app/space/view/render/renderer.cpp

// hpp
#include <surface/view/render/renderer.hpp>
// libv
#include <libv/glr/procedural/sphere.hpp>
#include <libv/glr/queue.hpp>
#include <libv/math/noise/white.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/event_hub.hpp>
#include <libv/ui/font_2D.hpp>
#include <libv/ui/ui.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/vm4/load.hpp>
// pro
//#include <surface/view/camera.hpp>
//#include <surface/command.hpp>
//#include <surface/game/game_instance.hpp>
//#include <surface/sim/playout/playout.hpp>
//#include <surface/sim/universe.hpp>
#include <surface/log.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

RendererDebug::RendererDebug(RendererResourceContext& rctx) :
		shader(rctx.shader_loader, "flat_color.vs", "flat_color.fs") {
}

void RendererDebug::build_points_mesh(libv::glr::Mesh& mesh) {
	mesh.clear();
	auto position = mesh.attribute(attribute_position);
	auto color0 = mesh.attribute(attribute_color0);
	auto index = mesh.index();
	libv::glr::VertexIndex vi = 0;

	for (const auto& point : points) {
		position(point.a);

		color0(point.color);

		index.point(vi);
		vi += 1;
	}
}

void RendererDebug::build_lines_mesh(libv::glr::Mesh& mesh) {
	mesh.clear();
	auto position = mesh.attribute(attribute_position);
	auto color0 = mesh.attribute(attribute_color0);
	auto index = mesh.index();
	libv::glr::VertexIndex vi = 0;

	for (const auto& line : lines) {
		position(line.a);
		position(line.b);

		color0(line.color);
		color0(line.color);

		index.line(vi + 0, vi + 1);
		vi += 2;
	}

//	for (const auto& sphere : spheres) {
//		//k0-1 -- k0
//		// |    / |
//		// |   /  |
//		// |  /   |
//		//k1-1 -- k1
//		const auto ring_step = libv::pi / static_cast<float>(sphere.ring_count);
//		const auto segment_step = libv::tau / static_cast<float>(sphere.segment_count);
//
//		for (int i = 0; i <= sphere.ring_count; ++i) {
//			const auto fi = static_cast<float>(i);
//			const auto u = libv::pi / 2.0f - fi * ring_step;
//			const auto z = sphere.radius * std::sin(u);
//
//			for (int j = 0; j <= sphere.segment_count; ++j) {
//				const auto fj = static_cast<float>(j);
//				const auto v = fj * segment_step;
//				const auto x = sphere.radius * std::cos(u) * std::cos(v);
//				const auto y = sphere.radius * std::cos(u) * std::sin(v);
//
//				libv::vec3f point = {x, y, z};
//				position(point + sphere.center);
//				color0({0.6f, 0, 1, 1});
//
//				const auto k1 = vi;
//				const auto k0 = k1 - sphere.segment_count - 1;
//				vi += 1;
//
//				if (i > 0 && j > 0) {
//					index.line(k0, k1);
//					index.line(k0 - 1, k0);
//				}
//			}
//		}
//	}

	//  7-------------6
	//  | \         / |
	//  |   \     /   |
	//  |    3---2    |
	//  |    |   |    |
	//  |    0---1    |
	//  |   /     \   |
	//  | /         \ |
	//  4-------------5
	for (const auto& frustum : frustums) {
		for (const auto& point : frustum.points) {
			position(point);
			color0(frustum.color_wire);
		}

		// Front-face
		index.line(vi + 0, vi + 1);
		index.line(vi + 1, vi + 2);
		index.line(vi + 2, vi + 3);
		index.line(vi + 3, vi + 0);

		index.line(vi + 4, vi + 5);
		index.line(vi + 5, vi + 6);
		index.line(vi + 6, vi + 7);
		index.line(vi + 7, vi + 4);

		index.line(vi + 0, vi + 4);
		index.line(vi + 1, vi + 5);
		index.line(vi + 2, vi + 6);
		index.line(vi + 3, vi + 7);

		vi += 8;
	}
}

void RendererDebug::build_triangles_mesh(libv::glr::Mesh& mesh) {
	mesh.clear();
	auto position = mesh.attribute(attribute_position);
	auto color0 = mesh.attribute(attribute_color0);
	auto index = mesh.index();
	libv::glr::VertexIndex vi = 0;

	for (const auto& triangle : triangles) {
		position(triangle.a);
		position(triangle.b);
		position(triangle.c);

		color0(triangle.color);
		color0(triangle.color);
		color0(triangle.color);

		index.triangle(vi + 0, vi + 1, vi + 2);
		index.triangle(vi + 0, vi + 2, vi + 1);
		vi += 3;
	}

	//  7-------------6
	//  | \         / |
	//  |   \     /   |
	//  |    3---2    |
	//  |    |   |    |
	//  |    0---1    |
	//  |   /     \   |
	//  | /         \ |
	//  4-------------5
	for (const auto& frustum : frustums) {
		for (const auto& point : frustum.points) {
			position(point);
			color0(frustum.color_sides);
		}

		// Front-face
		index.quad(vi + 0, vi + 1, vi + 2, vi + 3);
		index.quad(vi + 4, vi + 5, vi + 6, vi + 7);
		index.quad(vi + 4, vi + 5, vi + 1, vi + 0);
		index.quad(vi + 5, vi + 6, vi + 2, vi + 1);
		index.quad(vi + 6, vi + 7, vi + 3, vi + 2);
		index.quad(vi + 7, vi + 4, vi + 0, vi + 3);

		// Back-face
		index.quad(vi + 3, vi + 2, vi + 1, vi + 0);
		index.quad(vi + 7, vi + 6, vi + 5, vi + 4);
		index.quad(vi + 0, vi + 1, vi + 5, vi + 4);
		index.quad(vi + 1, vi + 2, vi + 6, vi + 5);
		index.quad(vi + 2, vi + 3, vi + 7, vi + 6);
		index.quad(vi + 3, vi + 0, vi + 4, vi + 7);
		vi += 8;
	}

	for (const auto& quad : quads) {
		position(quad.a);
		position(quad.b);
		position(quad.c);
		position(quad.d);

		color0(quad.color);
		color0(quad.color);
		color0(quad.color);
		color0(quad.color);

		index.quad(vi, vi + 1, vi + 2, vi + 3);
		index.quad(vi, vi + 3, vi + 2, vi + 1);
		vi += 4;
	}

	for (const auto& disc : discs) {
		const auto centeri = vi;
		position(disc.center);
		color0(disc.color);
		vi += 1;

		static constexpr auto count = 64;
		const auto up = libv::vec3f{0, 0, 1};
		const auto dot_prod = dot(disc.normal, up);
		const auto rotation_mat = dot_prod < 0.9999f ?
				libv::mat4f::identity().rotate(std::acos(dot_prod), cross(disc.normal, up)) :
				libv::mat4f::identity();
		for (int i = 0; i <= count; ++i) {
			float a = libv::tau / count * static_cast<float>(i);
			const auto x = disc.radius * std::cos(a);
			const auto y = disc.radius * std::sin(a);
			libv::vec3f point = {x, y, 0};
			auto rotated_point = rotation_mat * libv::vec4f{point, 1};
			position(xyz(rotated_point) + disc.center);
			color0(disc.color);

			if (i > 0) {
				index.triangle(vi - 1, centeri, vi);
				index.triangle(vi, centeri, vi - 1);
			}
			vi += 1;
		}
	}

	for (const auto& sphere : spheres) {
		//k0-1 -- k0
		// |    / |
		// |   /  |
		// |  /   |
		//k1-1 -- k1
		const auto ring_step = libv::pi / static_cast<float>(sphere.ring_count);
		const auto segment_step = libv::tau / static_cast<float>(sphere.segment_count);

		for (int i = 0; i <= sphere.ring_count; ++i) {
			const auto fi = static_cast<float>(i);
			const auto u = libv::pi / 2.0f - fi * ring_step;
			const auto z = sphere.radius * std::sin(u);

			for (int j = 0; j <= sphere.segment_count; ++j) {
				const auto fj = static_cast<float>(j);
				const auto v = fj * segment_step;
				const auto x = sphere.radius * std::cos(u) * std::cos(v);
				const auto y = sphere.radius * std::cos(u) * std::sin(v);

				libv::vec3f point = {x, y, z};
				position(point + sphere.center);
				color0(sphere.color);

				const auto k1 = vi;
				const auto k0 = k1 - sphere.segment_count - 1;
				vi += 1;

				if (i > 0 && j > 0) {
					index.triangle(k0 - 1, k1 - 1, k0);
					index.triangle(k0 - 1, k0, k1 - 1);
					index.triangle(k0, k1 - 1, k1);
					index.triangle(k0, k1, k1 - 1);
				}
			}
		}
	}
}

void RendererDebug::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
	// !!! P3: Rebuild is too expensive on each frame

	if (dirty) {
		build_points_mesh(mesh_point);
		build_lines_mesh(mesh_line);
		build_triangles_mesh(mesh_triangle);
		dirty = false;
	}

	glr.program(shader.program());

	{
		auto uniforms = uniform_stream.block_unique(layout_matrices);
		uniforms[layout_matrices.matMVP] = glr.mvp();
		uniforms[layout_matrices.matM] = glr.model;
		uniforms[layout_matrices.matP] = glr.projection;
		uniforms[layout_matrices.eye] = glr.eye();
		glr.uniform(std::move(uniforms));
		glr.render(mesh_triangle);
	}
	{
		auto uniforms = uniform_stream.block_unique(layout_matrices);
		uniforms[layout_matrices.matMVP] = glr.mvp();
		uniforms[layout_matrices.matM] = glr.model;
		uniforms[layout_matrices.matP] = glr.projection;
		uniforms[layout_matrices.eye] = glr.eye();
		glr.uniform(std::move(uniforms));
		glr.render(mesh_line);
	}
	{
		auto uniforms = uniform_stream.block_unique(layout_matrices);
		uniforms[layout_matrices.matMVP] = glr.mvp();
		uniforms[layout_matrices.matM] = glr.model;
		uniforms[layout_matrices.matP] = glr.projection;
		uniforms[layout_matrices.eye] = glr.eye();
		glr.uniform(std::move(uniforms));
		glr.render(mesh_point);
	}
}

void RendererDebug::add_debug_sphere(libv::vec3f center, float radius, libv::vec4f color, int ring_count, int segment_count) {
	spheres.emplace_back(center, radius, color, ring_count, segment_count);
	dirty = true;
}

void RendererDebug::clear_spheres() {
	spheres.clear();
	dirty = true;
}

void RendererDebug::add_debug_point(libv::vec3f a, libv::vec4f color) {
	points.emplace_back(a, color);
	dirty = true;
}

void RendererDebug::add_debug_line(libv::vec3f a, libv::vec3f b, libv::vec4f color) {
	lines.emplace_back(a, b, color);
	dirty = true;
}

void RendererDebug::add_debug_triangle(libv::vec3f a, libv::vec3f b, libv::vec3f c, libv::vec4f color) {
	triangles.emplace_back(a, b, c, color);
	dirty = true;
}

void RendererDebug::add_debug_circle(libv::vec3f center, float radius, libv::vec3f normal, libv::vec4f color) {
	discs.emplace_back(center, radius, normal, color);
	dirty = true;
}

void RendererDebug::add_debug_frustum(libv::vec3f nbl, libv::vec3f nbr, libv::vec3f ntr,
		libv::vec3f ntl, libv::vec3f fbl, libv::vec3f fbr,
		libv::vec3f ftr, libv::vec3f ftl, libv::vec4f color_wire, libv::vec4f color_sides) {
	frustums.push_back(RendererDebug::Frustum{{nbl, nbr, ntr, ntl, fbl, fbr, ftr, ftl}, color_sides, color_wire});
	dirty = true;
}

void RendererDebug::add_debug_quad(libv::vec3f a, libv::vec3f b, libv::vec3f c, libv::vec3f d, libv::vec4f color) {
	quads.emplace_back(a, b, c, d, color);
	dirty = true;
}

void RendererDebug::clear_debug_shapes() {
	points.clear();
	lines.clear();
	triangles.clear();
	quads.clear();
	frustums.clear();
	spheres.clear();
	dirty = true;
}

//void RendererDebug::renderTriangles(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
//	glr.program(shader.program());
//
//	{
//		auto uniforms = uniform_stream.block_unique(layout_matrices);
//		uniforms[layout_matrices.matMVP] = glr.mvp();
//		uniforms[layout_matrices.matM] = glr.model;
//		uniforms[layout_matrices.matP] = glr.projection;
//		uniforms[layout_matrices.eye] = glr.eye();
//		glr.uniform(std::move(uniforms));
//		glr.render(mesh_triangle);
//	}
//}

// -------------------------------------------------------------------------------------------------

RendererGizmo::RendererGizmo(RendererResourceContext& rctx) :
		shader(rctx.shader_loader, "flat_color.vs", "flat_color.fs") {
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

void RendererGizmo::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
	auto uniforms = uniform_stream.block_unique(layout_matrices);
	uniforms[layout_matrices.matMVP] = glr.mvp();
	uniforms[layout_matrices.matM] = glr.model;
	uniforms[layout_matrices.matP] = glr.projection;
	uniforms[layout_matrices.eye] = glr.eye();

	glr.program(shader.program());
	glr.uniform(std::move(uniforms));
	glr.render(mesh);
}

// -------------------------------------------------------------------------------------------------

RendererEditorGrid::RendererEditorGrid(RendererResourceContext& rctx) :
		shader(rctx.shader_loader, "editor_grid_plane.vs", "editor_grid_plane.fs") {
	auto position = mesh_grid.attribute(attribute_position);
	auto index = mesh_grid.index();

	position(-1, -1, 0);
	position(+1, -1, 0);
	position(+1, +1, 0);
	position(-1, +1, 0);

	index.quad(0, 1, 2, 3); // Front face quad
	index.quad(0, 3, 2, 1); // Back face quad
}

void RendererEditorGrid::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
	auto uniforms = uniform_stream.block_unique(layout_matrices);
	uniforms[layout_matrices.matMVP] = glr.mvp();
	uniforms[layout_matrices.matM] = glr.model;
	uniforms[layout_matrices.matP] = glr.projection;
	uniforms[layout_matrices.eye] = glr.eye();

	glr.program(shader.program());
	glr.uniform(std::move(uniforms));
	glr.render(mesh_grid);
}

// -------------------------------------------------------------------------------------------------

//RendererFleet::RendererFleet(RendererResourceContext& rctx) :
//// <<< P2: Model loader
//		model(libv::vm4::load_or_throw(libv::read_file_or_throw("../../res/model/tree_01.vm4"))),
////		model(libv::vm4::load_or_throw(libv::read_file_or_throw("../../res/model/Tree_med.fixed.game.vm4"))),
////		model(libv::vm4::load_or_throw(libv::read_file_or_throw("../../res/model/tank_01_rocket_ring.0031_med.game.vm4"))),
////		model(rctx.model_loader, "fighter_01_eltanin.0006_med.fixed.game.vm4"),
//		shader(rctx.shader_loader, "fleet.vs", "fleet.fs") {
//
////	log_surface.fatal("RendererFleet...");
////
////	log_surface.fatal("read_file_or_throw...");
////	auto file = libv::read_file_or_throw("../../res/model/fighter_01_eltanin.0006_med.fixed.game.vm4");
////	log_surface.fatal("load_or_throw...");
////	auto model_t = libv::vm4::load_or_throw(file);
////	// !!! exception from here gets silently ignored
////	log_surface.fatal("done...");
////
////	model.emplace(std::move(model_t));
//}
//
//void RendererFleet::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, Fleet::Selection selection_status) {
//	glr.program(shader.program());
//	glr.uniform(shader.uniform().base_color, libv::vec4f(0.7f, 0.7f, 0.7f, 1.0f));
//	glr.uniform(shader.uniform().selected, libv::to_underlying(selection_status));
//
//	model->render(glr, shader, uniform_stream);
//}

// -------------------------------------------------------------------------------------------------

RendererText::RendererText(RendererResourceContext& rctx) :
		shader(rctx.shader_loader, "font_2D.vs", "font_2D.fs") {
}

void RendererText::add_text(
		libv::vec3f position,
		libv::vec2f screenOffset,
		std::string str,
		libv::vec4f color,
		libv::ui::FontSize fontSize,
		libv::ui::AlignHorizontal alignH,
		libv::ui::AlignVertical alignV) {

	texts.emplace_back(
			position,
			screenOffset,
			color,
			libv::ui::TextLayout(
					font,
					fontSize,
					alignH,
					alignV,
					std::move(str)
			)
	);
}

void RendererText::clear_texts() {
	texts.clear();
}

void RendererText::add_debug_coordinates_if_nothing_else() {
	if (!texts.empty())
		return;

	add_text({0, 0, 0}, {0, 0}, "(0, 0, 0)");
	add_text({5, 0, 0}, {0, 0}, "(5, 0, 0)");
	add_text({0, 5, 0}, {0, 0}, "(0, 5, 0)");
	add_text({0, 0, 5}, {0, 0}, "(0, 0, 5)");
	add_text({-5, 0, 0}, {0, 0}, "(-5, 0, 0)", {1, 0.4f, 0.4f, 1});
	add_text({0, -5, 0}, {0, 0}, "(0, -5, 0)", {0.4f, 1, 0.4f, 1});
	add_text({0, 0, -5}, {0, 0}, "(0, 0, -5)", {0.4f, 0.4f, 1, 1});
}

void RendererText::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream, const CameraPlayer::screen_picker& screen_picker) {

//	auto& task = std::get<ImplContextRender::TaskUI>(context.tasks.emplace_back(
//			std::in_place_type<ImplContextRender::TaskUI>,
//			glr.state.state(),
//			shader->base_ref(),
//			font->texture().base_ref(),
//			context.uniform_stream.block_stream(layout_UIInfo),
//			static_cast<uint32_t>(base_vertex),
//			static_cast<uint32_t>(base_index),
//			static_cast<uint32_t>(num_index)
//	));
//
//	task.uniform_block[layout_UIInfo.matMVP] = glr.mvp().translate({pos, 0});
//	task.uniform_block[layout_UIInfo.matM] = glr.model.top().translate_copy({pos, 0});
//	task.uniform_block[layout_UIInfo.clip_pos] = clip_pos;
//	task.uniform_block[layout_UIInfo.clip_size] = clip_size;
//	task.uniform_block[layout_UIInfo.component_pos] = current_component.layout_position2();
//	task.uniform_block[layout_UIInfo.component_size] = current_component.layout_size2();
//	task.uniform_block[layout_UIInfo.mouse_position] = context.mouse_position;
//	task.uniform_block[layout_UIInfo.ui_size] = context.ui_size;
//	task.uniform_block[layout_UIInfo.time_frame] = context.current_time;

	const auto guard_p = glr.projection.push_guard();
	const auto guard_v = glr.view.push_guard();

	glr.projection = libv::mat4f::ortho({0, 0}, screen_picker.canvas_size(), -1000, +1000);
	glr.view = libv::mat4f::identity();

	{
		mesh.clear();

		auto position = mesh.attribute(attribute_position); // Character quad vertex positions
		//	texture0_tiles.insert(context.vtx_texture0_tiles.end(), num_vertex, libv::vec4f(0, 0, 1, 1));
		auto texture0 = mesh.attribute(attribute_texture0);
		auto color0 = mesh.attribute(attribute_color0);
		auto positionOffset = mesh.attribute(attribute_custom0); // Whole text position offset (so we don't have to use uniforms, and it can be a single pass)
		auto index = mesh.index();

		auto vi_offset = libv::glr::VertexIndex{0};
		for (auto& entry : texts) {
			const auto content_size = entry.layout.content(-1, -1);
			entry.layout.limit(content_size);
			const auto& vd = entry.layout.vertices_data();

			const auto alignment_offset =
					entry.screenOffset +
							content_size *
									libv::vec2f(
											-info(entry.layout.align_vertical()).rate(),
											-info(entry.layout.align_horizontal()).rate());

			const auto[position2DAnchor, onScreen] = screen_picker.to_screen_ahead(entry.position);
			if (!onScreen)
				continue;

			position.append_from_range(vd.positions);
			texture0.append_from_range(vd.texture0s);
			color0.append_n_times(vd.positions.size(), entry.color);
			positionOffset.append_n_times(vd.positions.size(), libv::vec4f{alignment_offset + position2DAnchor, 0, 0});
			index.append_from_range(vd.indices);

			for (auto& i : index.view_last(vd.indices.size()))
				i += vi_offset;

			vi_offset += static_cast<libv::glr::VertexIndex>(vd.positions.size());
		}
	}

	const auto guard_s = glr.state.push_guard();

	glr.state.blendSrc_Source1Color();
	glr.state.blendDst_One_Minus_Source1Color();
	glr.state.disableDepthTest();
	glr.state.disableDepthMask();

	auto uniforms = uniform_stream.block_unique(layout_matrices);
	uniforms[layout_matrices.matMVP] = glr.mvp();
	uniforms[layout_matrices.matM] = glr.model;
	uniforms[layout_matrices.matP] = glr.projection;
	uniforms[layout_matrices.eye] = glr.eye();

	glr.program(shader.program());
	glr.texture(font->texture().base_ref(), textureChannel_diffuse);
//	glr.uniform(shader.uniform().base_color, libv::vec4f(0.7f, 0.7f, 0.7f, 1.0f));
//	glr.uniform(shader.uniform().selected, selected);
	glr.uniform(std::move(uniforms));
	glr.render(mesh);
}

// -------------------------------------------------------------------------------------------------

RendererSurface::RendererSurface(RendererResourceContext& rctx) :
		shader(rctx.shader_loader, "flat_color.vs", "flat_color.fs") {}

void RendererSurface::addChunk(const surface::Chunk& chunk) {
	auto position = mesh.attribute(attribute_position);
	auto color0 = mesh.attribute(attribute_color0);
	auto index = mesh.index();

	const auto rowSize = chunk.height.size_y();
	for (unsigned int y = 0; y < rowSize; y++) {
		for (unsigned int x = 0; x < chunk.height.size_x(); ++x) {
			position(chunk.height(x, y).pos);
			color0(chunk.biomeMap(x, y));
		}
	}

	for (size_t i = 0; i < rowSize - 1; ++i) {
		index(vi);
		const auto colSize = chunk.height.size_x();
		const auto colSizeVi = static_cast<libv::glr::VertexIndex>(colSize);

		for (size_t j = 0; j < colSize; ++j) {
			index(vi);
			index(vi + colSizeVi);
			vi += 1;
		}
		index(vi + colSizeVi - 1);
	}
	vi += static_cast<libv::glr::VertexIndex>(rowSize);
}

void RendererSurface::clear() {
	mesh.clear();
	vi = 0;
}

void RendererSurface::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {

	glr.program(shader.program());
	{
		auto uniforms = uniform_stream.block_unique(layout_matrices);
		uniforms[layout_matrices.matMVP] = glr.mvp();
		uniforms[layout_matrices.matM] = glr.model;
		uniforms[layout_matrices.matP] = glr.projection;
		uniforms[layout_matrices.eye] = glr.eye();
		glr.uniform(std::move(uniforms));
		glr.render(mesh);
	}
}

// -------------------------------------------------------------------------------------------------


RendererSurfaceTexture::RendererSurfaceTexture(RendererResourceContext& rctx) :
		shader(rctx.shader_loader, "surface_texture.vs", "surface_texture.fs") {
//	build_mesh();
}

void RendererSurfaceTexture::build_mesh() {
	auto position = mesh.attribute(attribute_position);
	auto texture0 = mesh.attribute(attribute_texture0);
	auto index = mesh.index();

	position(0, 0, 0); //left-down
	texture0(0, 0);

	position(1, 0, 0); //right-down
	texture0(1, 0);

	position(1, 1, 0); //right-up
	texture0(1, 1);

	position(0, 1, 0); //left-up
	texture0(0, 1);

	index.quad(0, 1, 2, 3);
}

void RendererSurfaceTexture::addTexture(const libv::glr::Texture& texture, const libv::vec2f chunkPos, const libv::vec2f size) {
	const auto texture_pos = ChunkTexture{texture, chunkPos, size};
	chunks.emplace_back(texture_pos); //std::move?
}

void RendererSurfaceTexture::clear() {
	chunks.clear();
	mesh.clear();
}

void RendererSurfaceTexture::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
	build_mesh();

	glr.program(shader.program());

	for (const auto& chunk : chunks) {
		auto texture = chunk.texture;
		glr.texture(texture, textureChannel_diffuse);

		auto guard = glr.model.push_guard();
		glr.model.translate(libv::vec3f{chunk.pos, 0});
		glr.model.scale({chunk.size, 1});

		auto uniforms = uniform_stream.block_unique(layout_matrices);
		uniforms[layout_matrices.matMVP] = glr.mvp();
		uniforms[layout_matrices.matM] = glr.model;
		uniforms[layout_matrices.matP] = glr.projection;
		uniforms[layout_matrices.eye] = glr.eye();
		glr.uniform(std::move(uniforms));
		glr.render(mesh);
	}

}

// -------------------------------------------------------------------------------------------------

Renderer::Renderer(libv::ui::UI& ui) {
	resource_context.shader_loader.attach_libv_ui_hub(ui.event_hub());
	text.font = ui.context().font("consola.ttf");
}

void Renderer::prepare_for_render(libv::glr::Queue& glr) {
	// shader_loader.update MUST run before any other render queue operation
	// OTHERWISE the not loaded uniform locations are attempted to be used and placed into the streams
	// | So this is a better place than before, still not the best, When UI gets rev updates in the future maybe there will be better solutions
	resource_context.shader_loader.update(glr.out_of_order_gl());
}

// -------------------------------------------------------------------------------------------------

} // namespace surface
