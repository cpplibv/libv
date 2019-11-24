// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// hpp
#include <vm4_viewer/scene.hpp>
// libv
#include <libv/glr/queue.hpp>
#include <libv/math/angle.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

void Scene::render(libv::glr::Queue& gl, libv::vec2f canvas_size) {
	const auto guard_p = gl.projection.push_guard();
	const auto guard_v = gl.view.push_guard();
	const auto guard_m = gl.model.push_guard();
	const auto guard_s = gl.state.push_guard();

	gl.projection = camera.projection(canvas_size);
	gl.view = camera.view();
	gl.model = libv::mat4f::identity();

	gl.state.enableBlend();
	gl.state.blendSrc_SourceAlpha();
	gl.state.blendDst_One_Minus_SourceAlpha();

//		gl.state.disableCullFace();
	gl.state.enableCullFace();
	gl.state.frontFaceCCW();
	gl.state.cullBackFace();

	gl.state.polygonModeFill();
//		gl.state.polygonModeLine();

	gl.state.enableDepthTest();
	gl.state.depthFunctionLess();

	if (model)
		model->render(gl);
}

// -------------------------------------------------------------------------------------------------

} // namespace app
