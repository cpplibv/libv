// File: panel.cpp, Created on 2015. április 8. 14:32, Author: Vader

// vl
#include <ui/font/font_2D.hpp>
// ext
#include <GL/glew.h>
// pro
#include "vl/ui/log.hpp"
#include "vl/ui/component/label.hpp"



namespace vl {
namespace ui {

Font2D deafultFont;

void Label::setText(const std::string& text) {
	this->text = text;
	invalidate();
}

void Label::build() {
	VLOG_TRACE(vl::ui::log(), "Build Label");
	deafultFont.load("Data/Font/cour.ttf", 12);
}

void Label::destroy() {
	VLOG_TRACE(vl::ui::log(), "Destroy Label");
	deafultFont.clean();
}

void Label::render() {
	//VLOG_TRACE(vl::ui::log(), "Render Label");

	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.17f, 0.09f);
	glVertex2f(-1, -1);
	glVertex2f(1, -1);
	glVertex2f(1, 1);
	glVertex2f(-1, 1);
	glEnd();
	
	deafultFont.begin();
	deafultFont.print(-10, -10, text.c_str());
	deafultFont.end();
}

} //namespace ui
} //namespace vl
