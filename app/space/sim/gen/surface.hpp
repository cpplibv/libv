// Created by dbobula on 1/16/2022.

#pragma once

#include <iostream>
#include <libv/math/vec.hpp>
#include <libv/container/vector_2d.hpp>

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
	int size;
	std::vector<std::vector<SurfacePoint>> points;
//	libv::vector_2D<SurfacePoint> points{256, 256};
//	libv::vec4f color;

};

class SurfaceGen{
public:
//	gimme_surface();
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


