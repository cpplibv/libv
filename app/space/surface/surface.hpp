// Created by dbobula on 1/16/2022.

#pragma once

//multithreading
#include <libv/mt/hardware_concurrency.hpp>
#include <libv/mt/thread_bulk.hpp>
#include <libv/utility/read_file.hpp>

#include <libv/math/vec.hpp>
#include <libv/container/vector_2d.hpp>

// std
#include <array>

#include <space/surface/config.hpp>
#include <space/surface/node.hpp>

#include <libv/math/gradient.hpp>




namespace surface {

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
	[[nodiscard]] std::vector<libv::vec4f> getColors() {
		std::vector<libv::vec4f> colors;
		colors.reserve(size * size);
		for (const auto& row : points) {
			for (const auto& point : row) {
				colors.emplace_back(point.color);
			}
		}
		return colors;
	}

};



class NoiseGen {
private:
	libv::mt::thread_bulk threads{libv::mt::hardware_concurrency_or(4) - 2};
//	sol::state lua;
//	Config config;

public:
//	gimme_surface();
	NoiseGen();
	Surface generateNoise(const Config& config);

};


//class Tile{
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

} // namespace surface

