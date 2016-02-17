// File: panel.cpp, Created on 2015. április 8. 14:32, Author: Vader

// hpp
#include <libv/ui/container/panel.hpp>
// ext
#include <glm/gtx/transform.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

void BasePanel::doBuild(Renderer& gl) {
	Container::buildComponents(gl);
	Component::validate();
}

void BasePanel::doDestroy(Renderer& gl) {
	Container::destroyComponents(gl);
}

void BasePanel::doRender(Renderer& gl) {
	gl.pushMatrixView();
	gl.matrixView() *= glm::translate(glm::vec3(displayPosition.x, displayPosition.y, displayPosition.z));
	// vec-glm
	Container::renderComponents(gl);
	gl.popMatrixView();
}

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv
