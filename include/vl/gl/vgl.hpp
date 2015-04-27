// File: vgl.hpp, Created on 2014. november 25. 12:14, Author: Vader

#pragma once

// ext
#include <GL/glew.h>
#include <glm/glm.hpp>
// std
#include <stack>
// pro
#include "vl/gl/log.hpp"

//#define MEMBER_OFFSET(s,m) (nullptr + (offsetof(s,m)))
//#define BUFFER_OFFSET(i) (nullptr + (i))
#define MEMBER_OFFSET(s,m) ((char *)NULL + (offsetof(s,m)))
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

//Math

#define PI 3.1415926535897932384626433832795f

namespace vl {
	namespace gl {

		glm::mat4& MMat();
		glm::mat4& VMat();
		glm::mat4& PMat();
		glm::mat4& TMat();
		glm::mat4 MVPMat();

		void popMMat();
		void popVMat();
		void popPMat();

		void pushMMat();
		void pushVMat();
		void pushPMat();

		////FrameBuffer
		//extern GLuint depthFrameBuffer;
		//extern GLuint depthTexture;
		//
		////Shader
		//extern GLuint simpleShader;
		//extern GLuint lightShader;
		//extern GLuint light2Shader;
		//extern GLuint shaderDepthCast;
		//
		////Light
		//class Light;
		//extern vector<Light*> glLight;

		//Debug
		void renderCube(float x = 0, float y = 0, float z = 0, float size = 0.5f);
		void renderXYZ(float x = 0, float y = 0, float z = 0, float size = 2);
	} //namespace gl
} //namespace vl
