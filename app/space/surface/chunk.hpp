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
#include <map>

#include <space/surface/config.hpp>
#include <space/surface/node.hpp>

#include <libv/math/gradient.hpp>
#include <libv/utility/random/xoroshiro128.hpp> // fwdable
#include <libv/utility/random/uniform_distribution.hpp>


namespace surface {

struct SurfacePoint {
	libv::vec3f pos;
	libv::vec4f color;
//	bool hasTree = false;
};

//struct TexturePoint {
//	libv::vec2f pos;
//	libv::vec4f color;
//};

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

// no public by defualt unless dumb data (no invariant), add constructor
class Chunk {
public:
	Chunk(const size_t size_, const libv::vec2f position_);
private:
	size_t size;
public:
	libv::vec2f position;
	libv::vector_2D<SurfacePoint> height;
	libv::vector_2D<SurfacePoint> temperature;
	libv::vector_2D<SurfacePoint> humidity;
	libv::vector_2D<SurfacePoint> fertility;
	std::vector<SurfaceObjectStorage> featureList;
public:
	float getHeight(const libv::vec2f position);

	[[nodiscard]] static std::vector<libv::vec4f> getColors(const libv::vector_2D<SurfacePoint>& points_);
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

	[[nodiscard]] Chunk generateChunk(const Config& yi, const libv::vec2f chunkPosition);
//	[[nodiscard]] libv::vector_2D<TexturePoint> generateTexturePoints();
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

