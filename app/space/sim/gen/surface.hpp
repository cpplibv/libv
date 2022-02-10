// Created by dbobula on 1/16/2022.

#pragma once

#include <iostream>
#include <libv/math/vec.hpp>

// std
#include <array>


// libv




//
//
//
//

struct SurfacePoint {
public:
	libv::vec3f point;
	libv::vec4f color;
};

class Surface {
public:
	std::array<std::array<SurfacePoint, 100>, 100> points;
//	libv::vec4f color;

};



//class LandStrip{
//public:
//	Biome biome_type;
//
//};
//
//class Biome{
//	Vegetation vegetation;
//	Weather weather_conditions;
//
//};


