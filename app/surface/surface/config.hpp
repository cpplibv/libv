// Created by dbobula on 2/20/2022.

#pragma once

#include <surface/surface/node.hpp>
//#include <surface/surface/biome.hpp>

#include <libv/noise/noise.hpp>
#include <libv/math/gradient.hpp>
#include <libv/math/vec.hpp>
#include <libv/container/flat_set.hpp>


namespace surface {

enum class Visualization {
	model,
	spheres
};

enum class SurfaceObjectType {
	tree,
	bush,
	rock
};

enum class NodeType {
	perlin,
	warp,
	add
};

enum class NoiseType {
	value,
	perlin,
	simplex,
	simplex2S,
	cellular
};

//struct Fractal{
//	//fractal
//	int octaves;
//	float amplitude;
//	float frequency;
//	float lacunarity;
//	float persistence;
//};

enum class PlantDistribution {
	random,
	clustered
};

struct SurfaceObject {
	SurfaceObjectType type;
	float size;
	size_t count;
	libv::vec4f color;
};

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

struct VeggieType {
	std::string name;
	float probability = 1.0f;
	float size = 1.0f;
	libv::vec3f pos;
	libv::vec4f color;


//	Range<float> fertility;
//	std::string path;
//	Range<float> height;
//	//hsv diff color: saturation, value mehet, hue 20-30 fok vec3f(hue, saturation, value)
//	Range<float> hue;
//	Range<float> saturation;
//	Range<float> value;

};

struct Biome {
	std::string name;
	libv::vec2f coord;
	libv::vec2f cutOff;
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




class Config {
public:
	Visualization visualization;
//	std::string currentHeatMap;
	size_t resolution; /// Number of quads/pixels per side
	size_t numChunks; /// Number of quads/pixels per side
	size_t numVeggie;
	float amplitude;
	PlantDistribution plantDistribution;
	int circleNumber;
	float circleSize;
	std::vector<SurfaceObject> objects;
	libv::flat_set<Biome> biomes;
//	libv::gradientf<libv::vec4f> colorGrad;
//	std::unique_ptr<Node> rootNode;
	HeatMap height;
	HeatMap temperature;
	HeatMap humidity;
	HeatMap fertility;

//	std::unique_ptr<Node> heightRootNode;
//	std::unique_ptr<Node> temperatureOffsetRootNode;
//	std::unique_ptr<Node> humidityRootNode;
//	std::unique_ptr<Node> fertilityRootNode;

	//fractal
//	int octaves;
//	float amplitude;
//	float frequency;
//	float lacunarity;
//	float persistence;
//	//noise
//	int seed;
//	NoiseType noiseType;
//
//	//cellular
//	libv::CellularDistanceFunction distanceFn = libv::CellularDistanceFunction::euclidean;
//	libv::CellularReturnType returnType = libv::CellularReturnType::cellValue;
//	float jitter = 1.0f;

};
} // namespace surface

