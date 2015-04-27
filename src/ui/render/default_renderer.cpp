// File: DefaultRenderer.cpp, Created on 2014. április 24. 12:16, Author: Vader

// hpp
#include "vl/ui/render/default_renderer.hpp"
// ext
#include <GL/glew.h>
// pro
#include "vl/ui/component/quad.hpp"

namespace vl {
namespace ui {

void DefaultRenderer::operator()(const Quad* v) {
	ivec3 size = v->getDisplaySize();
	ivec3 pos = v->getDisplayPosition();

	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glViewport(pos.x, pos.y, size.x, size.y);
	gluOrtho2D(0, size.x, 0, size.y);

	glBegin(GL_TRIANGLE_STRIP);
	glColor4f(v->id, 0, v->blue, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(size.x, 0, 0);
	glVertex3f(0, size.y, 0);
	glVertex3f(size.x, size.y, 0);
	glEnd();
	glPopMatrix();
}

} //namespace ui
} //namespace vl
