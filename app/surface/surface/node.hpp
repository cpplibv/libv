// Created by dbobula on 3/1/2022.

#pragma once

// ext
#include <boost/container/small_vector.hpp>
// libv
#include <libv/math/vec.hpp>
#include <libv/noise/noise.hpp>
#include <libv/utility/approx.hpp>
#include <libv/math/mix.hpp>
#include <libv/math/step.hpp>
// std
#include <memory>
#include <cmath>


namespace surface {

// -------------------------------------------------------------------------------------------------

using Seed = uint32_t;

// -------------------------------------------------------------------------------------------------

struct Node {
	virtual float evaluate(float x, float y) = 0;
	virtual ~Node() = default;
};

// -------------------------------------------------------------------------------------------------

struct NodeConstant : Node {
	float value = 0.f;

	virtual float evaluate(float x, float y) override;
};

struct NodeValue : Node {
	Seed seed = 0x5EED;

	virtual float evaluate(float x, float y) override {
		return libv::noise_value(seed, x, y);
	}
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

struct NodeFractal : Node {
	std::unique_ptr<Node> input;
	Seed seed = 0x5EED;
	int octaves = 1;
	float amplitude = 1.0;
	float frequency = 1.0f;
	float lacunarity = 2.0f;
	float persistence = 0.5f;

	explicit NodeFractal(std::unique_ptr<Node> input) : input(std::move(input)) {}

	virtual float evaluate(float x, float y) override {
		Seed not_used_seed = 0;
		return libv::noise_fractal(not_used_seed, x, y, [&](Seed, float x0, float y0) {
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

struct NodeCoordinate : Node {
	libv::vec2f weights;

	virtual float evaluate(float x, float y) override {
		return weights.x * x + weights.y * y;
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


struct NodeMix : Node {
	std::unique_ptr<Node> a;
	std::unique_ptr<Node> b;
	std::unique_ptr<Node> t;

	NodeMix(std::unique_ptr<Node> a, std::unique_ptr<Node> b, std::unique_ptr<Node> t) :
			a(std::move(a)), b(std::move(b)), t(std::move(t)) {}

	virtual float evaluate(float x, float y) override {
		const auto t_ = t->evaluate(x,y);
		if(libv::approx(t_) == 0.f)
			return a->evaluate(x,y);
		else if(libv::approx(t_) == 1.f)
			return b->evaluate(x,y);
		else
			return libv::mix(a->evaluate(x, y), b->evaluate(x, y), t_);
	}
};

struct NodeClamp : Node {
	std::unique_ptr<Node> value;
	std::unique_ptr<Node> low;
	std::unique_ptr<Node> high;

	NodeClamp(std::unique_ptr<Node> value, std::unique_ptr<Node> low, std::unique_ptr<Node> high) :
			value(std::move(value)), low(std::move(low)), high(std::move(high)) {}

	virtual float evaluate(float x, float y) override {
		const auto value_ = value->evaluate(x,y);
		const auto low_ = low->evaluate(x,y);
		const auto high_ = high->evaluate(x,y);

		return std::clamp(value_, low_, high_);
	}
};


//TODO [0-1] koze besurit
struct NodeSaturate : Node {
	std::unique_ptr<Node> input;

	explicit NodeSaturate(std::unique_ptr<Node> input) :
		input(std::move(input)) {}

	virtual float evaluate(float x, float y) override {
		return std::clamp(input->evaluate(x,y), 0.f, 1.f);
	}
};

struct NodeMin : Node {
	std::unique_ptr<Node> value;
	std::unique_ptr<Node> min;

	NodeMin(std::unique_ptr<Node> value, std::unique_ptr<Node> min) :
	value(std::move(value)), min(std::move(min)) {}

	virtual float evaluate(float x, float y) override {
		return std::min(value->evaluate(x,y), min->evaluate(x, y));
	}
};

struct NodeMax : Node {
	std::unique_ptr<Node> value;
	std::unique_ptr<Node> max;

	NodeMax(std::unique_ptr<Node> value, std::unique_ptr<Node> max) :
			value(std::move(value)), max(std::move(max)) {}

	virtual float evaluate(float x, float y) override {
		return std::max(value->evaluate(x,y), max->evaluate(x, y));
	}
};

struct NodeSin : Node {
	std::unique_ptr<Node> input;

	explicit NodeSin(std::unique_ptr<Node> input) :
			input(std::move(input)) {}

	virtual float evaluate(float x, float y) override {
		return std::sin(input->evaluate(x,y));
	}
};

struct NodeSmoothstep : Node {
	std::unique_ptr<Node> value;
	std::unique_ptr<Node> edge0;
	std::unique_ptr<Node> edge1;

	NodeSmoothstep(std::unique_ptr<Node> value, std::unique_ptr<Node> edge0, std::unique_ptr<Node> edge1) :
	value(std::move(value)), edge0(std::move(edge0)), edge1(std::move(edge1)) {}

	virtual float evaluate(float x, float y) override {
		const auto value_ = value->evaluate(x,y);
		const auto edge0_ = edge0->evaluate(x,y);
		const auto edge1_ = edge1->evaluate(x,y);

		return libv::smoothstep(value_, edge0_, edge1_);
	}
};

//TODO if node
//struct NodeIf : Node {
//	std::unique_ptr<Node> value;
//	std::unique_ptr<Node> low;
//	std::unique_ptr<Node> high;
//
//	NodeClamp(std::unique_ptr<Node> value, std::unique_ptr<Node> low, std::unique_ptr<Node> high) :
//			value(std::move(value)), low(std::move(low)), high(std::move(high)) {}
//
//	virtual float evaluate(float x, float y) override {
//		const auto value_ = value->evaluate(x,y);
//		const auto low_ = low->evaluate(x,y);
//		const auto high_ = high->evaluate(x,y);
//
//		return std::clamp(value_, low_, high_);
//	}
//};


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

// -------------------------------------------------------------------------------------------------

} // namespace surface
