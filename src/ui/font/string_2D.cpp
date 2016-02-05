// File: Font2D.cpp, Created on 2014. november 30. 14:32, Author: Vader

// hpp
#include <libv/ui/font/string_2D.hpp>
// ext
#include <boost/filesystem/path.hpp>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <utf8cpp/utf8.h>
// libv
#include <libv/ui/log.hpp>
#include <libv/gl/log.hpp>
#include <libv/gl/gl.hpp>
// pro
#include <libv/ui/font/font_2D.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

String2D::String2D(const std::string& text, Anchor halign, Anchor valign) :
	halign(halign),
	valign(valign),
	rawText(text) { }


// -------------------------------------------------------------------------------------------------

void String2D::setText(const std::string& text) {
	rawText = text;
}

void String2D::setSize(ivec2 size) {
	this->size = size;
}

// -------------------------------------------------------------------------------------------------

void String2D::build() {
	if (!deafultFont.isLoaded()) {
		LIBV_UI_COMPONENT_TRACE("Load Font");
		deafultFont.load("Data/Font/cour.ttf");
	}

	buildImpl();
}

void String2D::destroy() {
	if (deafultFont.isLoaded()) {
		LIBV_UI_COMPONENT_TRACE("Unload Font");
		deafultFont.unload();
	}
}

void String2D::render() {
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
}

// -------------------------------------------------------------------------------------------------

void String2D::buildImpl() {
	data.clear();

	auto iter = rawText.begin();
	auto end = rawText.end();

	while (iter != end) {
		auto unicode = utf8::next(iter, end); // TODO P2: handle 'invalid' utf8 exceptions

		// ... Next commit ...

	}
}

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv
