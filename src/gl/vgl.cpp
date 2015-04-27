// File: vgl.cpp, Created on 2014. november 25. 12:14, Author: Vader

// hpp
#include "vl/gl/vgl.hpp"
// ext
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace vl {
	namespace gl {
		glm::mat4 glTmat(0.5, 0.0, 0.0, 0.0,
				0.0, 0.5, 0.0, 0.0,
				0.0, 0.0, 0.5, 0.0,
				0.5, 0.5, 0.5, 1.0);

		std::stack<glm::mat4> MMatStack({glm::mat4(1.0f)});
		std::stack<glm::mat4> VMatStack({glm::mat4(1.0f)});
		std::stack<glm::mat4> PMatStack({glm::mat4(1.0f)});

		glm::mat4& MMat() {
			return MMatStack.top();
		}

		glm::mat4& VMat() {
			return VMatStack.top();
		}

		glm::mat4& PMat() {
			return PMatStack.top();
		}

		glm::mat4& TMat() {
			return glTmat;
		}

		glm::mat4 MVPMat() {
			return PMatStack.top() * VMatStack.top() * MMatStack.top();
		}

		void popMMat() {
			MMatStack.pop();
		}

		void popVMat() {
			VMatStack.pop();
		}

		void popPMat() {
			PMatStack.pop();
		}

		void pushMMat() {
			MMatStack.push(MMatStack.top());
		}

		void pushVMat() {
			VMatStack.push(VMatStack.top());
		}

		void pushPMat() {
			PMatStack.push(PMatStack.top());
		}

		void renderCube(float x, float y, float z, float size) {

			glBegin(GL_QUADS);

			//Right
			glNormal3f(1.0f, 0.0f, 0.0f);
			glVertex3f(x + size, y + size, z + size);
			glVertex3f(x + size, y - size, z + size);
			glVertex3f(x + size, y - size, z - size);
			glVertex3f(x + size, y + size, z - size);

			//Left
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glVertex3f(x - size, y - size, z - size);
			glVertex3f(x - size, y - size, z + size);
			glVertex3f(x - size, y + size, z + size);
			glVertex3f(x - size, y + size, z - size);

			//Top
			glNormal3f(0.0f, 1.0f, 0.0f);
			glVertex3f(x + size, y + size, z + size);
			glVertex3f(x + size, y + size, z - size);
			glVertex3f(x - size, y + size, z - size);
			glVertex3f(x - size, y + size, z + size);

			//Bottom
			glNormal3f(0.0f, -1.0f, 0.0f);
			glVertex3f(x - size, y - size, z - size);
			glVertex3f(x + size, y - size, z - size);
			glVertex3f(x + size, y - size, z + size);
			glVertex3f(x - size, y - size, z + size);

			//Near
			glNormal3f(0.0f, 0.0f, -1.0f);
			glVertex3f(x + size, y + size, z - size);
			glVertex3f(x + size, y - size, z - size);
			glVertex3f(x - size, y - size, z - size);
			glVertex3f(x - size, y + size, z - size);

			//Far
			glNormal3f(0.0f, 0.0f, 1.0f);
			glVertex3f(x + size, y + size, z + size);
			glVertex3f(x - size, y + size, z + size);
			glVertex3f(x - size, y - size, z + size);
			glVertex3f(x + size, y - size, z + size);

			glEnd();
		}

		void renderXYZ(float x, float y, float z, float size) {
			glBegin(GL_LINES);
			glColor3f(1, 0, 0);
			glVertex3f(x, y, z);
			glVertex3f(x + size, y, z);
			glColor3f(0, 1, 0);
			glVertex3f(x, y, z);
			glVertex3f(x, y + size, z);
			glColor3f(0, 0, 1);
			glVertex3f(x, y, z);
			glVertex3f(x, y, z + size);
			glEnd();
			glColor3f(1, 1, 1);
		}

	} //namespace gl
} //namespace vl









//<editor-fold defaultstate="collapsed" desc="rand">
//		void initRand() {
//			srand(time(0));
//		}
//		int randD(int x) {
//			return (int) ((float) rand() / (float) RAND_MAX * x + 0.5);
//		}
//		float randF(float x) {
//			return (float) rand() / ((float) RAND_MAX / x);
//		}
//		float randRange(float min, float max) {
//			return (float) rand() / (float) RAND_MAX * (max - min) + min;
//		}
//		glm::vec3 randUnitVec() {
//			float a = randF(2 * PI), b = randRange(-1, 1);
//			return glm::vec3(sqrt(1 - b * b) * cos(a), sqrt(1 - b * b) * sin(a), b);
//		}
//		float getDir(float a, float b) { //get the dir from origo to (a,b) point //performance: 1 var , 2-3-4 if , 2 = , 1 atan
//			float dir;
//			if (b != 0) { //null oszt
//				dir = 57.6 * atan(a / b);
//				if (0 < a)
//					if (0 < b)
//						dir = 90 - dir;
//					else
//						dir = 270 - dir;
//				else if (0 < b)
//					dir = 90 - dir;
//				else
//					dir = 270 - dir;
//			} else if (a < 0)
//				dir = 180;
//			else
//				dir = 0;
//
//			return dir;
//		}// </editor-fold>
