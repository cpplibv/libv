// File: vgl.cpp, Created on 2014. november 25. 12:14, Author: Vader

// hpp
#include <vl/gl/gl.hpp>
// ext
#include <GL/glew.h>

namespace vl {
namespace gl {

void renderCube(float x, float y, float z, float size) {
	glBegin(GL_QUADS);

	//Right
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(x + size, y - size, z - size);
	glTexCoord2f(0, 1);
	glVertex3f(x + size, y - size, z + size);
	glTexCoord2f(1, 1);
	glVertex3f(x + size, y + size, z + size);
	glTexCoord2f(1, 0);
	glVertex3f(x + size, y + size, z - size);

	//Left
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(x - size, y - size, z - size);
	glTexCoord2f(0, 1);
	glVertex3f(x - size, y + size, z - size);
	glTexCoord2f(1, 1);
	glVertex3f(x - size, y + size, z + size);
	glTexCoord2f(1, 0);
	glVertex3f(x - size, y - size, z + size);

	//Top
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(x - size, y + size, z - size);
	glTexCoord2f(0, 1);
	glVertex3f(x + size, y + size, z - size);
	glTexCoord2f(1, 1);
	glVertex3f(x + size, y + size, z + size);
	glTexCoord2f(1, 0);
	glVertex3f(x - size, y + size, z + size);

	//Bottom
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(x - size, y - size, z - size);
	glTexCoord2f(0, 1);
	glVertex3f(x - size, y - size, z + size);
	glTexCoord2f(1, 1);
	glVertex3f(x + size, y - size, z + size);
	glTexCoord2f(1, 0);
	glVertex3f(x + size, y - size, z - size);

	//Near
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(0, 0);
	glVertex3f(x - size, y - size, z - size);
	glTexCoord2f(0, 1);
	glVertex3f(x + size, y - size, z - size);
	glTexCoord2f(1, 1);
	glVertex3f(x + size, y + size, z - size);
	glTexCoord2f(1, 0);
	glVertex3f(x - size, y + size, z - size);

	//Far
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0, 0);
	glVertex3f(x - size, y - size, z + size);
	glTexCoord2f(0, 1);
	glVertex3f(x - size, y + size, z + size);
	glTexCoord2f(1, 1);
	glVertex3f(x + size, y + size, z + size);
	glTexCoord2f(1, 0);
	glVertex3f(x + size, y - size, z + size);

	glEnd();
}

//void renderXYZ(float x, float y, float z, float size) {
//	glBegin(GL_LINES);
//	glColor3f(1, 0, 0);
//	glVertex3f(x, y, z);
//	glVertex3f(x + size, y, z);
//	glColor3f(0, 1, 0);
//	glVertex3f(x, y, z);
//	glVertex3f(x, y + size, z);
//	glColor3f(0, 0, 1);
//	glVertex3f(x, y, z);
//	glVertex3f(x, y, z + size);
//	glEnd();
//	glColor3f(1, 1, 1);
//}

} //namespace gl
} //namespace vl