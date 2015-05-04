// File: Light.hpp, Created on 2014. december 19. 13:09, Author: Vader

#pragma once

#include <glm/glm.hpp>

namespace vl {
namespace gl {
// Light ---------------------------------------------------------------

struct LightType {
	static const int point = 0;
	static const int dir = 1;
	static const int spot = 2;
};

struct Light {
	int type;
	bool enabled; //0 false 1 true

	glm::dvec3 position;
	glm::dvec3 direction;
	glm::vec4 diffuse;
	glm::vec4 specular;

	double range;
	double intensity;
	double innerCosAngle; // Angles closer to 1 produce tighter cones
	double outerCosAngle; // Angles of -1 will emulate point lights.

	bool shadowCast; //0 false 1 true
	//			GLuint frameBuffer;
	//			GLuint shadowDepthTexture; //Texture sampler for shadow map. The textureSamplers layout is 10+i where 'i' is the index of the light!
	//			GLuint shadowMapSampler;
	unsigned int frameBuffer;
	unsigned int shadowDepthTexture; //Texture sampler for shadow map. The textureSamplers layout is 10+i where 'i' is the index of the light!
	unsigned int shadowMapSampler;
	glm::dmat4 shadowMVPTmat; //MVPT mat

	glm::dmat4 getVmat();
	glm::dmat4 getPmat();
	Light();
	~Light();
};

} //namespace gl
} //namespace vl