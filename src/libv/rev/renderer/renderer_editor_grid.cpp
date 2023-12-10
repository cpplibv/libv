// Project: libv.rev, File: src/libv/rev/renderer/renderer_editor_grid.cpp

// hpp
#include <libv/rev/renderer/renderer_editor_grid.hpp>
// libv
#include <libv/glr/queue.hpp>
#include <libv/glr/uniform_buffer.hpp>
// pro
#include <libv/rev/resource_manager.hpp>
#include <libv/rev/shader/attribute.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

RendererEditorGrid::RendererEditorGrid(libv::rev::ResourceManager& loader) :
		shader(loader.shader, "editor/editor_grid.vs", "editor/editor_grid.fs") {
	auto position = mesh_grid.attribute(libv::rev::attribute_position);
	auto index = mesh_grid.index();

	position(-1, -1, 0);
	position(+1, -1, 0);
	position(+1, +1, 0);
	position(-1, +1, 0);

	index.quad(0, 1, 2, 3); // Up faceing quad
	index.quad(0, 3, 2, 1); // Down faceing quad
}

void RendererEditorGrid::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) {
	auto matrices = uniform_stream.block_unique(libv::rev::layout_Matrices200);
	matrices[libv::rev::layout_Matrices200.matMVP] = glr.mvp();
	matrices[libv::rev::layout_Matrices200.matM] = glr.model;

	auto camera = uniform_stream.block_unique(libv::rev::layout_Camera200);
	camera[libv::rev::layout_Camera200.matP] = glr.projection;
	camera[libv::rev::layout_Camera200.matV] = glr.view;
	camera[libv::rev::layout_Camera200.eye] = glr.eye();
	camera[libv::rev::layout_Camera200.cameraForwardW] = libv::vec3f(glr.view.top()[0][2], glr.view.top()[1][2], glr.view.top()[2][2]);
	camera[libv::rev::layout_Camera200.cameraRightW] = libv::vec3f(glr.view.top()[0][0], glr.view.top()[1][0], glr.view.top()[2][0]);
	camera[libv::rev::layout_Camera200.cameraUpW] = libv::vec3f(glr.view.top()[0][1], glr.view.top()[1][1], glr.view.top()[2][1]);

	glr.program(shader.program());
	glr.uniform(std::move(matrices));
	glr.uniform(std::move(camera));
	glr.render(mesh_grid);
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
