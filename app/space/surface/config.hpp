// Created by dbobula on 2/20/2022.

#pragma once

#include <libv/noise/noise.hpp>
#include <space/surface/node.hpp>
#include <libv/math/gradient.hpp>
#include <libv/math/vec.hpp>


namespace surface {

enum class Mode {
	texture,
	_3d
};

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
	std::unique_ptr<Node> rootNode;
};

struct Config {
	Mode mode;
	Visualization visualization;
	size_t resolution; /// Number of quads/pixels per side
	float amplitude;
	PlantDistribution plantDistribution;
	int circleNumber;
	float circleSize;
	std::vector<SurfaceObject> objects;
	libv::gradientf<libv::vec4f> colorGrad;
	std::unique_ptr<Node> rootNode;
	std::vector<HeatMap> heatMaps;
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

