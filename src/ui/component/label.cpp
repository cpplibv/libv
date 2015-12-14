// File: panel.cpp, Created on 2015. április 8. 14:32, Author: Vader

// hpp
#include <vl/ui/component/label.hpp>
// ext
#include <boost/filesystem/path.hpp>
#include <GL/glew.h>
// pro
#include <vl/ui/log.hpp>
#include <vl/gl/log.hpp>

namespace vl {
namespace ui {

void Label::setText(const std::string& text) {
	this->text = text;
	invalidate();
}

void Label::build(Renderer&) {
	VLOG_TRACE(vl::ui::log(), "Build Label");
	if (!deafultFont.isLoaded())
		deafultFont.load("Data/Font/cour.ttf");
}

void Label::destroy(Renderer&) {
	VLOG_TRACE(vl::ui::log(), "Destroy Label");
	if (deafultFont.isLoaded())
		deafultFont.unload();
}

void Label::render(Renderer& renderer) {
	//	VLOG_TRACE(vl::ui::log(), "Render Label");

	(void) renderer;
	//	gluOrtho2D(0, 512, 0, 512);
	glEnable(GL_TEXTURE_2D);
	//<<< This is an articaft here due to vui_main should already set up this
	deafultFont.bind();
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glTexCoord2f(0, 0);
	glVertex2f(-1.0f, -1.0f);
	glTexCoord2f(1, 0);
	glVertex2f(+1.0f, -1.0f);
	glTexCoord2f(1, 1);
	glVertex2f(+1.0f, +1.0f);
	glTexCoord2f(0, 1);
	glVertex2f(-1.0f, +1.0f);
	glEnd();
	deafultFont.unbind();

	checkGL();

	//	deafultFont.bind();
	//	deafultFont.print(-0.10f, -0.10f, text.c_str());
	//	deafultFont.unbind();
}

} //namespace ui
} //namespace vl
