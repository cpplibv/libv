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

#include <surface/surface/config.hpp>
#include <surface/surface/biome.hpp>
#include <surface/surface/node.hpp>

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

class QueryResult {
	float height;
	float temp;
	float wet;
	float fertility;
};

class BiomeCell {
	libv::vec4f color;
	Biome config;
};

// no public by defualt unless dumb data (no invariant), add constructor
class Chunk {
private:
	size_t size;
//	size_t biomeCellNumber;
	libv::xoroshiro128 rng;

public:
	libv::vec2f position;
//	libv::vector_2D<BiomeCell> biomeMap;
	libv::vector_2D<libv::vec4f> biomeMap;

	libv::vector_2D<SurfacePoint> height;
	libv::vector_2D<float> temperature; // TODO P1: Only store a float, surface point is overkill
	libv::vector_2D<float> humidity;
	libv::vector_2D<float> fertility;
	libv::vector_2D<float> temp_humidity_distribution;
//	std::vector<SurfaceObjectStorage> featureList;
	std::vector<VeggieType> veggies;

public:
//	Chunk(const size_t size_, const size_t biomeCellNumber_, const libv::vec2f position_);
	Chunk(const size_t size_, const libv::vec2i index, const libv::vec2f position_);
public:
	void placeVegetationClustered(const Config& config);
	void placeVegetationRandom(const Config& config);

	[[nodiscard]] static float getHeight(const libv::vec2f position, const libv::vector_2D<SurfacePoint>& heatMap);
//	[[nodiscard]] float getInterpolatedValue(const libv::vec2f position, const libv::vector_2D<float>& heatMap);

//	[[nodiscard]] static std::vector<libv::vec4f> getColors(const libv::vector_2D<SurfacePoint>& points_);
};

// -------------------------------------------------------------------------------------------------

class ChunkGen {
private:
	libv::mt::thread_bulk threads{libv::mt::hardware_concurrency_or(4) - 2};
	libv::xoroshiro128 rng{123};



//	surface::Renderer renderer;
//	sol::state lua;
//	Config config;

public:
//	gimme_surface();
//	explicit ChunkGen(surface::Renderer& renderer);
	ChunkGen();

	[[nodiscard]] Chunk generateChunk(const Config& yi, const libv::vec2i chunkPosition);
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

