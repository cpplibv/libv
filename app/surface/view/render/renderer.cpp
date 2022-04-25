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

RendererGizmo::RendererGizmo(RendererResourceContext& rctx) :
		shader(rctx.shader_loader, "flat_color.vs", "flat_color.fs") {
	build_gizmo_lines(mesh);
}

void RendererGizmo::build_gizmo_lines(Mesh& mesh) {
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

//RendererSurface::RendererSurface(RendererResourceContext& rctx) :
//		shader(rctx.shader_loader, "flat_color.vs", "flat_color.fs") {}
//
//void RendererSurface::addChunk(const surface::Chunk& chunk) {
//	auto position = mesh.attribute(attribute_position);
//	auto color0 = mesh.attribute(attribute_color0);
//	auto index = mesh.index();
//
//	const auto rowSize = chunk.height.size_y();
//	for (unsigned int y = 0; y < rowSize; y++) {
//		for (unsigned int x = 0; x < chunk.height.size_x(); ++x) {
//			position(chunk.height(x, y).pos);
//			color0(chunk.biomeMap(x, y));
//		}
//	}
//
//	for (size_t i = 0; i < rowSize - 1; ++i) {
//		index(vi);
//		const auto colSize = chunk.height.size_x();
//		const auto colSizeVi = static_cast<libv::glr::VertexIndex>(colSize);
//
//		for (size_t j = 0; j < colSize; ++j) {
//			index(vi);
//			index(vi + colSizeVi);
//			vi += 1;
//		}
//		index(vi + colSizeVi - 1);
//	}
//	vi += static_cast<libv::glr::VertexIndex>(rowSize);
//}
//
//void RendererSurface::clear() {
//	mesh.clear();
//	vi = 0;
//}
//
//void RendererSurface::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
//
//	glr.program(shader.program());
//	{
//		auto uniforms = uniform_stream.block_unique(layout_matrices);
//		uniforms[layout_matrices.matMVP] = glr.mvp();
//		uniforms[layout_matrices.matM] = glr.model;
//		uniforms[layout_matrices.matP] = glr.projection;
//		uniforms[layout_matrices.eye] = glr.eye();
//		glr.uniform(std::move(uniforms));
//		glr.render(mesh);
//	}
//}


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
