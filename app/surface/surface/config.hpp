// Created by dbobula on 2/20/2022.

#pragma once

// libv
#include <libv/container/flat_set.hpp>
#include <libv/math/gradient.hpp>
#include <libv/math/vec.hpp>
// std
#include <memory>
// pro
#include <surface/surface/fwd.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

enum class NodeType {
	perlin,
	warp,
	add,
};

enum class NoiseType {
	value,
	perlin,
	simplex,
	simplex2S,
	cellular,
};

enum class PlantDistribution {
	random,
	clustered,
};

// -------------------------------------------------------------------------------------------------

struct HeatMap {
	std::string name;
	float heightSensitivity;
//	libv::vec4f color;
	libv::gradientf<libv::vec4f> colorGrad;
	std::unique_ptr<Node> rootNode;
};

template <typename T>
struct Range {
	T min;
	T max;
};

template <typename T>
struct Shift {
	T offset;
	T radius;
};

template <typename T>
struct Limit {
	T soft;
	T hard;
};

struct VeggieType {
	std::string name;
	float probability = 1.0f;
//	float size = 1.0f;

//	Range<float> fertility;
	std::string path;
	Range<float> scale;

/// hsv diff color: saturation, value mehet, hue 20-30 fok vec3f(hue, saturation, value)
	Shift<float> hue;
	Shift<float> saturation;
	Shift<float> value;
};

struct Biome {
	std::string name;
	libv::vec2f coord;
	Limit<float> takeover; //take over, monopol, seize, exclude
	Limit<float> handover; //
//	libv::vec2f cutOff;
	float dominance = 1.f;
	libv::gradientf<libv::vec4f> colorGrad;
	std::vector<VeggieType> vegetation;

	friend inline bool operator<(const Biome& lhs, const Biome& rhs) noexcept {
		return lhs.name < rhs.name;
	}

	friend inline bool operator<(const Biome& lhs, const std::string& rhs) noexcept {
		return lhs.name < rhs;
	}

	friend inline bool operator<(const std::string& lhs, const Biome& rhs) noexcept {
		return lhs < rhs.name;
	}
};

// -------------------------------------------------------------------------------------------------

class Config {
public:
	Seed globalSeed;

	bool blendBiomes = true;

	size_t resolution; /// Number of quads/pixels per side
	size_t numChunks;
	size_t numVeggie;

	PlantDistribution plantDistribution;

	float fogIntensity;
	libv::vec4f fogColor;

	libv::flat_set<Biome> biomes;

	HeatMap height;
	HeatMap temperature;
	HeatMap humidity;
	HeatMap fertility;
};

// -------------------------------------------------------------------------------------------------

} // namespace surface
