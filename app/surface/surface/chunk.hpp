// Created by dbobula on 1/16/2022.

#pragma once

// libv
#include <libv/container/vector_2d.hpp>
#include <libv/math/vec.hpp>
#include <libv/mt/hardware_concurrency.hpp>
#include <libv/mt/thread_bulk.hpp>
#include <libv/utility/random/xoroshiro128.hpp>
// std
#include <surface/surface/fwd.hpp>
#include <surface/surface/veggie.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

//enum class ChunkState {
//	pending,
//	ready,
//	active,
//	expired,
//};

class Chunk {
public:
	libv::vec2i index;
	libv::vec2f position;
	libv::vec2f size;
	uint32_t resolution;
	libv::xoroshiro128 rng;

//	ChunkState state = ChunkState::pending;

public:
	libv::vector_2D<libv::vec3f> height; // Could be a single float, XY could be derived from index in vertex shader
	libv::vector_2D<libv::vec3f> normal;
	libv::vector_2D<libv::vec4f> color;

	libv::vector_2D<float> temperature;
	libv::vector_2D<float> humidity;
	libv::vector_2D<float> fertility;
//	libv::vector_2D<float> temp_humidity_distribution;
	std::vector<Veggie> veggies; // TODO P4: Rename to objects

public:
	Chunk(libv::vec2i index, libv::vec2f position_, libv::vec2f size, uint32_t resolution, uint32_t globalSeed);

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
	void placeVegetation(const Config& config, Chunk& chunk);

private:
	void placeVegetationClustered(const Config& config, Chunk& chunk);
	void placeVegetationRandom(const Config& config, Chunk& chunk);
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
