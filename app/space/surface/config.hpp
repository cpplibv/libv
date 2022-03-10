// Created by dbobula on 2/20/2022.

#pragma once

#include <libv/noise/noise.hpp>
#include <space/surface/node.hpp>
#include <libv/math/gradient.hpp>
#include <libv/math/vec.hpp>

namespace surface {

enum class Mode{
	texture,
	_3d
};

enum class NodeType{
	perlin,
	warp,
	add
};

enum class NoiseType{
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

struct Config {
	Mode mode;
	int size;
	float amplitude;
	libv::gradientf<libv::vec4f> colorGrad;
	std::unique_ptr<Node> rootNode;

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

