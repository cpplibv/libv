// Project: libv, File: app/vm4_viewer/scene/grid.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <vm4_viewer/scene/grid.hpp>
// libv
//#include <libv/glr/procedural/cube.hpp>
#include <libv/glr/procedural/grid.hpp>
//#include <libv/glr/procedural/ignore.hpp>
//#include <libv/glr/procedural/sphere.hpp>
#include <libv/glr/queue.hpp>
#include <libv/math/angle.hpp>
// pro
#include <vm4_viewer/attribute.hpp>
//#include <vm4_viewer/log.hpp>
//#include <vm4_viewer/ui/keyboard.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

Grid::Grid(ShaderLoader& shader_loader) :
	shader_grid(shader_loader,
			libv::gl::ShaderType::Vertex, "grid.vs",
			libv::gl::ShaderType::Fragment, "grid.fs"
	),

	mesh(libv::gl::Primitive::Lines, libv::gl::BufferUsage::StaticDraw) {

	{
		auto pos = mesh.attribute(attribute_position);
		auto index = mesh.index();
//		libv::glr::generateGrid(25, 25, pos, libv::glr::ignore, index);
	}
}

void Grid::render(libv::glr::Queue& gl, libv::vec2f canvas_size) {
	const auto guard_p = gl.projection.push_guard();
	const auto guard_v = gl.view.push_guard();
	const auto guard_m = gl.model.push_guard();
	const auto guard_s = gl.state.push_guard();

	gl.state.enableBlend();
	gl.state.blendSrc_SourceAlpha();
	gl.state.blendDst_One_Minus_SourceAlpha();

	// gl.state.disableCullFace();
	// gl.state.polygonModeFill();

	gl.state.enableDepthTest();
	gl.state.depthFunctionLess();

	{
		const auto guard_m = gl.model.push_guard();
		const auto guard_s = gl.state.push_guard();

		gl.program(shader_grid.program);
		gl.uniform(shader_grid.uniform.matMVP, gl.mvp());
		gl.uniform(shader_grid.uniform.eye, gl.eye());
		gl.uniform(shader_grid.uniform.color, color_grid);
		gl.render(mesh);
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace app
