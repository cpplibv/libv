// File: panel.cpp, Created on 2015. április 8. 14:32, Author: Vader

// hpp
#include <libv/ui/component/label.hpp>
// ext
//#include <boost/filesystem/path.hpp>
//#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
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
}

void Label::doDestroy(Renderer&) {
	LIBV_UI_COMPONENT_TRACE("Destroy Label");
	text.destroy();
}

void Label::doRender(Renderer& gl) {
	//	LIBV_UI_COMPONENT_TRACE("Render Label");

	const auto position = getDisplayPosition();
	const auto size = getDisplaySize();

	gl.pushMatrixView(glm::ortho(
			position.x, position.x + size.x,
			position.y, position.y + size.y,
			1000.f, -1000.f));

	text.render();

	checkGL();
}

} //namespace ui
} //namespace libv
