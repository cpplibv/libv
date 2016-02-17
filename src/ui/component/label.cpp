// File: panel.cpp, Created on 2015. április 8. 14:32, Author: Vader

// hpp
#include <libv/ui/component/label.hpp>
// ext
#include <glm/gtx/transform.hpp>
// pro
#include <libv/ui/log.hpp>
#include <libv/gl/log.hpp>

namespace libv {
namespace ui {

void Label::setText(const std::string& text) {
	LIBV_UI_COMPONENT_TRACE("Set label text to [%s]", text);
	this->text.setText(text);
	invalidate();
}

void Label::doBuild(Renderer&) {
	LIBV_UI_COMPONENT_TRACE("Build Label");
	text.build();
	set(Property::Size, vec3(text.getLayoutedSize(), 0));
}

void Label::doDestroy(Renderer&) {
	LIBV_UI_COMPONENT_TRACE("Destroy Label");
	text.destroy();
}

void Label::doRender(Renderer& gl) {
	//LIBV_UI_COMPONENT_TRACE("Render Label");

	const auto position = getDisplayPosition();
	const auto size = getDisplaySize();
	LIBV_UI_LAYOUT_TRACE("LayoutNFO [%s]: pos(%.1f,%.1f,%.1f), size(%.1f,%.1f,%.1f)",
			componentID, position.x, position.y, position.z, size.x, size.y, size.z);

	gl.pushMatrixModel();
	gl.matrixModel() *= glm::translate(glm::vec3(position.x, position.y, position.z)); // vec-glm

	text.render(gl);

	gl.popMatrixModel();
	checkGL();
}

} //namespace ui
} //namespace libv
