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
#include <libv/utility/random/xoroshiro128.hpp> // fwdable
#include <libv/utility/random/uniform_distribution.hpp>


namespace surface {

struct SurfacePoint {
	libv::vec3f point;
	libv::vec4f color;
//	bool hasTree = false;
};

struct SurfaceObjectPoint {
	libv::vec3f position;
	float size;
	libv::vec4f color;

//	bool hasTree = false;
};

//enum class VegetationType{
//	rock,
//	grass,
//	bush,
//	tree
//};

struct SurfaceObjectStorage {
	std::vector<SurfaceObjectPoint> points;
//	libv::vec4f color;
	SurfaceObjectType type;
};

class Chunk {
// no public by defualt unless dumb data (no invariant), add constructor
public:
	int size;
	libv::vec2f position;
//	std::vector<std::vector<SurfacePoint>> points;
	libv::vector_2D<SurfacePoint> points{256, 256}; //constructor
	std::vector<SurfaceObjectStorage> featureList;

//	libv::vec4f color;
	[[nodiscard]]std::vector<libv::vec4f> getColors() const {
		std::vector<libv::vec4f> colors;
		colors.reserve(points.size_x() * points.size_y());
		for (int y = 0; y < points.size_y(); ++y) {
			for (int x = 0; x < points.size_x(); ++x) {
				colors.emplace_back(points(x, y).color);
			}
		}
		return colors;
	}
};

// -------------------------------------------------------------------------------------------------

class ChunkGen {
private:
	libv::mt::thread_bulk threads{libv::mt::hardware_concurrency_or(4) - 2};
	libv::xoroshiro128 range{123};

	void placeVegetationClustered(Chunk& chunk, const Config& config);
	void placeVegetationRandom(Chunk& chunk, const Config& config);

//	space::Renderer renderer;
//	sol::state lua;
//	Config config;

public:
//	gimme_surface();
//	explicit ChunkGen(space::Renderer& renderer);
	ChunkGen();

	Chunk generateChunk(const Config& yi, const libv::vec2f chunkPosition);
	void placeVegetation(Chunk& chunk, const Config& config);
//		void placeVegetation(const Config& config);

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

