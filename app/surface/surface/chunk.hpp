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

//#include <libv/math/gradient.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

struct SurfacePoint {
	libv::vec3f pos;
//	float height;
	libv::vec4f color;
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

enum class ChunkState {
	pending,
	ready,
	active,
	expired,
};

class Chunk {
public:
	libv::vec2i index;
	libv::vec2f position;
	libv::vec2f size;
	uint32_t resolution;
	libv::xoroshiro128 rng;

	ChunkState state = ChunkState::pending;

public:
//	libv::vector_2D<BiomeCell> biomeMap;
	libv::vector_2D<libv::vec4f> biomeMap;

	libv::vector_2D<SurfacePoint> height;
	libv::vector_2D<float> temperature; // TODO P1: Only store a float, surface point is overkill
	libv::vector_2D<float> humidity;
	libv::vector_2D<float> fertility;
	libv::vector_2D<float> temp_humidity_distribution;
//	std::vector<SurfaceObjectStorage> featureList;
	std::vector<Veggie> veggies;

public:
	Chunk(libv::vec2i index, libv::vec2f position_, libv::vec2f size, uint32_t resolution, Seed globalSeed);

public:
	/// @param uv The relative local (uv) coordinates (in region of 0..1) of the requested point
	[[nodiscard]] float getInterpolatedHeight(const libv::vec2f uv) const;
	[[nodiscard]] libv::vec3f pickRandomPoint(libv::xoroshiro128& rng) const;
//	[[nodiscard]] float getInterpolatedValue(const libv::vec2f position, const libv::vector_2D<float>& heatMap);

//	[[nodiscard]] static std::vector<libv::vec4f> getColors(const libv::vector_2D<SurfacePoint>& points_);
};

// -------------------------------------------------------------------------------------------------

class ChunkGen {
private:
	libv::mt::thread_bulk threads{libv::mt::hardware_concurrency_or(4) - 2};

public:
	ChunkGen();

	void generateChunk(const Config& config, Chunk& chunk);
//	libv::vector_2D<TexturePoint> generateTexturePoints();
	void placeVegetation(const Config& config, Chunk& chunk);

private:
	void placeVegetationClustered(const Config& config, Chunk& chunk);
	void placeVegetationRandom(const Config& config, Chunk& chunk);
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
