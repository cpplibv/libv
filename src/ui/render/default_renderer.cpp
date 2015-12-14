// File: DefaultRenderer.cpp, Created on 2014. április 24. 12:16, Author: Vader

// hpp
#include <libv/ui/render/default_renderer.hpp>
// ext
#include <GL/glew.h>
// pro
#include <libv/ui/component/quad.hpp>

namespace libv {
namespace ui {

void DefaultRenderer::operator()(const Quad* v) {
	ivec3 isize = v->getDisplaySize();
	ivec3 ipos = v->getDisplayPosition();

	vec3 size = v->getDisplaySize();

	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glViewport(ipos.x, ipos.y, isize.x, isize.y);
	gluOrtho2D(0.f, size.x, 0.f, size.y);

	glBegin(GL_TRIANGLE_STRIP);
	glColor4f(v->id, 0.f, v->blue, 1.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(size.x, 0.f, 0.f);
	glVertex3f(0.f, size.y, 0.f);
	glVertex3f(size.x, size.y, 0.f);
	glEnd();
	glPopMatrix();
}

} //namespace ui
} //namespace libv