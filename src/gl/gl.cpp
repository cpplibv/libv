// File: vgl.cpp, Created on 2014. november 25. 12:14, Author: Vader

// hpp
#include <libv/gl/gl.hpp>
// ext
#include <GL/glew.h>

namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

const char* to_string(ShaderType type) {
	switch (type) {
	default:
	case ShaderType::Vertex: return SHADER_TYPE_VERTEX_NAME;
	case ShaderType::Geometry: return SHADER_TYPE_GEOMETRY_NAME;
	case ShaderType::Fragment: return SHADER_TYPE_FRAGMENT_NAME;
	case ShaderType::Compute: return SHADER_TYPE_COMPUTE_NAME;
	case ShaderType::TessControl: return SHADER_TYPE_TESS_CONTROL_NAME;
	case ShaderType::TessEvaluation: return SHADER_TYPE_TESS_EVALUATION_NAME;
	}
}

// -------------------------------------------------------------------------------------------------

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

// -------------------------------------------------------------------------------------------------

} //namespace gl
} //namespace libv