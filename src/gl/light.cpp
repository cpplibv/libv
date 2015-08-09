// File: Light.cpp, Created on 2014. december 19. 13:12, Author: Vader

#include <vl/gl/light.hpp>


namespace vl {
	namespace gl {
		// Light ---------------------------------------------------------------

		Light::Light() :
			type(LightType::point),
			enabled(true),
			position(0, 0, 0),
			direction(1, 0, 0),
			diffuse(1, 1, 1, 1),
			specular(1.0f, 1.0f, 1.0f, 1.0f),
			range(75.0f),
			intensity(1.0f),
			innerCosAngle(0.8f),
			outerCosAngle(0.6f),
			shadowCast(false) { }

	} //namespace gl
} //namespace vl