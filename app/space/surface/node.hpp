// Created by dbobula on 3/1/2022.

#pragma once

//#include <vector>
#include <memory>
#include <cmath>

#include <libv/noise/noise.hpp>

#include <boost/container/small_vector.hpp>


namespace surface {
using Seed = uint32_t;

struct Node {
	virtual float evaluate(float x, float y) = 0;
	virtual ~Node() = default;
};

struct NodeConstant : Node {
	float value = 0.f;

	virtual float evaluate(float x, float y) override;
};

struct NodePerlin : Node {
	Seed seed = 0x5EED;

	virtual float evaluate(float x, float y) override {
		return libv::noise_perlin(seed, x, y);
	}
};

struct NodeSimplex : Node {
	Seed seed = 0x5EED;

	virtual float evaluate(float x, float y) override {
		return libv::noise_simplex(seed, x, y);
	}
};

struct NodeCellular : Node {
	Seed seed = 0x5EED;

	libv::CellularDistanceFunction distanceFn = libv::CellularDistanceFunction::euclidean;
	libv::CellularReturnType returnType = libv::CellularReturnType::cellValue;
	float jitter = 1.0f;

	virtual float evaluate(float x, float y) override {
		return libv::noise_cellular(seed, x, y, distanceFn, returnType, jitter);
	}
};

struct BaseFractalNode : Node {
	Seed seed = 0x5EED;
	int octaves = 1;
	float amplitude = 1.0;
	float frequency = 1.0f;
	float lacunarity = 2.0f;
	float persistence = 0.5f;
	virtual float evaluate(float x, float y) override = 0;
};

struct NodeSimplexFractal : BaseFractalNode {
	virtual float evaluate(float x, float y) override {
		return libv::noise_fractal(seed, x, y, libv::noise_simplex<float>, octaves, amplitude, frequency, lacunarity, persistence);
	}
};

struct FractalNode : Node {
	std::unique_ptr<Node> input;

	Seed seed = 0x5EED; //not used
	int octaves = 1;
	float amplitude = 1.0;
	float frequency = 1.0f;
	float lacunarity = 2.0f;
	float persistence = 0.5f;

	virtual float evaluate(float x, float y) override {
		return libv::noise_fractal(seed, x, y, [&](Seed seed, float x0, float y0) {
					return input->evaluate(x0, y0);
				},
				octaves, amplitude, frequency, lacunarity, persistence);
	}
};

struct NodeWarp : BaseFractalNode {
	std::unique_ptr<Node> input;

	//	Node* input;
	explicit NodeWarp(std::unique_ptr<Node> input) : input(std::move(input)) {}

	virtual float evaluate(float x, float y) override {
		libv::warpWithIndependentFractal(seed, x, y, amplitude, frequency, octaves, lacunarity, persistence);
		return input->evaluate(x, y);
	}
};

struct NodePlus : Node {
	using container = boost::container::small_vector<std::unique_ptr<Node>, 4>;
	container inputs;

//	Node* input0;
//	Node* input1;
//
//	virtual float evaluate(float x, float y) override {
//		return
//				input0->evaluate(x, y) +
//						input1->evaluate(x, y);
//	}
	explicit NodePlus(container inputs) : inputs(std::move(inputs)) {}

	virtual float evaluate(float x, float y) override;
};

struct NodeMul : Node {
	using container = boost::container::small_vector<std::unique_ptr<Node>, 4>;
	container inputs;

//	Node* input0;
//	Node* input1;
//
//	virtual float evaluate(float x, float y) override {
//		return
//				input0->evaluate(x, y) +
//						input1->evaluate(x, y);
//	}
	explicit NodeMul(container inputs) : inputs(std::move(inputs)) {}

	virtual float evaluate(float x, float y) override;
};

struct NodePow : Node {
	std::unique_ptr<Node> input;

	float exponent = 1.f;

	explicit NodePow(std::unique_ptr<Node> input) : input(std::move(input)) {}

	virtual float evaluate(float x, float y) override;
};

//TODO P2: MixNode b

//TODO P4: NodeOutput might not be necessary for current scope
//struct NodeOutput : Node {
//	std::unique_ptr<Node> input;
//	float output;
//
//	virtual float evaluate(float x, float y) override {
//		const auto result = input->evaluate(x, y);
//		output = result;
//		return result;
//	}
//};

//TODO: multiply node

//TODO P5: NodeSplit definitely out of scope for now
//struct NodeSplit : Node {
//    Node* input;
//    Node* output;
//
//    virtual float evaluate(float x, float y) override {
//        const auto result = input->evaluate(x, y);
//        output = result;
//        return result;
//    }
//};



} // namespace surface
