// File: panel.cpp, Created on 2015. április 8. 14:32, Author: Vader

// hpp
#include <vl/ui/component/label.hpp>
// ext
#include <GL/glew.h>
// pro
#include <vl/ui/log.hpp>

namespace vl {
namespace ui {

void Label::setText(const std::string& text) {
	this->text = text;
	invalidate();
}

void Label::build(Renderer& renderer) {
	VLOG_TRACE(vl::ui::log(), "Build Label");

	deafultFont.load("Data/Font/cour.ttf", 12);
}

void Label::destroy(Renderer&) {
	VLOG_TRACE(vl::ui::log(), "Destroy Label");
	deafultFont.clean();
}

void Label::render(Renderer& renderer) {
	//VLOG_TRACE(vl::ui::log(), "Render Label");

//	glBegin(GL_QUADS);
//	glColor3f(0.50f, 0.17f, 0.09f);
//	glVertex2f(-0.10f, -0.10f);
//	glVertex2f(0.10f, -0.10f);
//	glVertex2f(0.10f, 0.10f);
//	glVertex2f(-0.10f, 0.10f);
//	glEnd();

	deafultFont.begin();
	deafultFont.print(-0.10f, -0.10f, text.c_str());
	deafultFont.end();
}

} //namespace ui
} //namespace vl
