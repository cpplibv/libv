// Created by dbobula on 2/20/2022.

#pragma once

#include <libv/noise/noise.hpp>


namespace surface {

enum class Mode{
	texture,
	_3d
};

enum class NoiseType{
	value,
	perlin,
	simplex,
	simplex2S,
	cellular
};

struct Config {
	Mode mode;
	int size;
	//fractal
	int octaves;
	float amplitude;
	float frequency;
	float lacunarity;
	float persistence;
	//noise
	int seed;
	NoiseType noiseType;

	//cellular
	libv::CellularDistanceFunction distanceFn = libv::CellularDistanceFunction::euclidean;
	libv::CellularReturnType returnType = libv::CellularReturnType::cellValue;
	float jitter = 1.0f;

};
} // namespace surface

