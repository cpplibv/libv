// Created by dbobula on 3/1/2022.

#pragma once

// ext
#include <boost/container/small_vector.hpp>
// libv
#include <libv/math/vec.hpp>
#include <libv/noise/noise.hpp>
// std
#include <memory>
// pro
#include <surface/surface/fwd.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

struct Node {
	virtual float evaluate(float x, float y) noexcept = 0;
	virtual ~Node() = default;
};

// -------------------------------------------------------------------------------------------------

struct NodeConstant : Node {
	float value = 0.f;

	virtual float evaluate(float x, float y) noexcept override;
};

struct NodeValue : Node {
	Seed seed = 0x5EED;

	virtual float evaluate(float x, float y) noexcept override;
};

struct NodePerlin : Node {
	Seed seed = 0x5EED;

	virtual float evaluate(float x, float y) noexcept override;
};

struct NodeSimplex : Node {
	Seed seed = 0x5EED;

	virtual float evaluate(float x, float y) noexcept override;
};

struct NodeCellular : Node {
	Seed seed = 0x5EED;

	libv::CellularDistanceFunction distanceFn = libv::CellularDistanceFunction::euclidean;
	libv::CellularReturnType returnType = libv::CellularReturnType::cellValue;
	float jitter = 1.0f;

	virtual float evaluate(float x, float y) noexcept override;
};

struct BaseFractalNode : Node {
	Seed seed = 0x5EED;
	int octaves = 1;
	float amplitude = 1.0;
	float frequency = 1.0f;
	float lacunarity = 2.0f;
	float persistence = 0.5f;

	virtual float evaluate(float x, float y) noexcept override = 0;
};

struct NodeSimplexFractal : BaseFractalNode {
	virtual float evaluate(float x, float y) noexcept override;
};

struct NodeFractal : BaseFractalNode {
	std::unique_ptr<Node> input;

	explicit inline NodeFractal(std::unique_ptr<Node> input) :
			input(std::move(input)) {}

	virtual float evaluate(float x, float y) noexcept override;

//	virtual int32_t glsl(ScriptGLSL& glsl, std::string_view coordinates) override {
//		const auto resultID = glsl.nextVarID();
//		const auto gainID = glsl.nextVarID();
//		const auto iterCoordID = glsl.nextCoordID();
//		glsl.outln("float v{} = 0;", resultID);
//		glsl.outln("float v{} = {};", gainID, amplitude);
//		glsl.outln("vec2 c{} = c{};", iterCoordID, coordinates);
//		glsl.outln("for (int i = 0; i < {}; ++i) {{", octaves);
//		{
//			auto t = glsl.tab_guard();
//			glsl.outln("c{} = ...;", iterCoordID, lacunarity);
//			auto inputResultID = input->glsl(glsl, iterCoordID);
//			glsl.outln("v{} = v{};", resultID, inputResultID);
//			glsl.outln("v{} *= {};", gainID, persistence);
//			...
//		}
//		glsl.outln("}}", resultID);
//		return resultID;
//	}
};

struct NodeWarp : BaseFractalNode {
	std::unique_ptr<Node> input;

	explicit inline NodeWarp(std::unique_ptr<Node> input) :
			input(std::move(input)) {}

	virtual float evaluate(float x, float y) noexcept override;

//	virtual int32_t glsl(ScriptGLSL& glsl, std::string_view coordinates) override {
//		const auto resultCoordID = glsl.nextCoordID();
//		glsl.outln("vec2 c{} = warpFractalIndependent({});", resultCoordID, coordinates);
//		return input->glsl(glsl, "c{}", resultCoordID);
//	}
};

struct NodeCoordinate : Node {
	libv::vec2f weights;

	virtual float evaluate(float x, float y) noexcept override;

//	virtual int32_t glsl(ScriptGLSL& glsl, std::string_view coordinates) override {
//		const auto resultID = glsl.nextVarID();
//		glsl.outln("float v{} = {}.x * {} + {}.y * {};", resultID, coordinates, weights.x, coordinates, weights.y);
//		return resultID;
//	}
};

struct NodePlus : Node {
	using container = boost::container::small_vector<std::unique_ptr<Node>, 4>;
	container inputs;

	explicit inline NodePlus(container inputs) :
			inputs(std::move(inputs)) {}

	virtual float evaluate(float x, float y) noexcept override;

//	virtual int32_t glsl(ScriptGLSL& glsl, std::string_view coordinates) override {
//		const auto boost::container::small_vector<GLSLVarID, 4>;
//		const auto resultID = glsl.nextVarID();
//		glsl.outln("float v{} = {}.x * {} + {}.y * {};", resultID, coordinates, weights.x, coordinates, weights.y);
//		return resultID;
//	}
};

struct NodeMul : Node {
	using container = boost::container::small_vector<std::unique_ptr<Node>, 4>;
	container inputs;

	explicit inline NodeMul(container inputs) :
			inputs(std::move(inputs)) {}

	virtual float evaluate(float x, float y) noexcept override;
};

struct NodePow : Node {
	std::unique_ptr<Node> input;

	float exponent = 1.f;

	explicit inline NodePow(std::unique_ptr<Node> input) :
			input(std::move(input)) {}

	virtual float evaluate(float x, float y) noexcept override;
};

struct NodeMix : Node {
	std::unique_ptr<Node> a;
	std::unique_ptr<Node> b;
	std::unique_ptr<Node> t;

	inline NodeMix(std::unique_ptr<Node> a, std::unique_ptr<Node> b, std::unique_ptr<Node> t) :
			a(std::move(a)),
			b(std::move(b)),
			t(std::move(t)) {}

	virtual float evaluate(float x, float y) noexcept override;
};

struct NodeClamp : Node {
	std::unique_ptr<Node> value;
	std::unique_ptr<Node> low;
	std::unique_ptr<Node> high;

	inline NodeClamp(std::unique_ptr<Node> value, std::unique_ptr<Node> low, std::unique_ptr<Node> high) :
			value(std::move(value)),
			low(std::move(low)),
			high(std::move(high)) {}

	virtual float evaluate(float x, float y) noexcept override;
};

struct NodeSaturate : Node {
	std::unique_ptr<Node> input;

	explicit inline NodeSaturate(std::unique_ptr<Node> input) :
			input(std::move(input)) {}

	virtual float evaluate(float x, float y) noexcept override;
};

struct NodeMin : Node {
	std::unique_ptr<Node> value;
	std::unique_ptr<Node> min;

	inline NodeMin(std::unique_ptr<Node> value, std::unique_ptr<Node> min) :
			value(std::move(value)),
			min(std::move(min)) {}

	virtual float evaluate(float x, float y) noexcept override;
};

struct NodeMax : Node {
	std::unique_ptr<Node> value;
	std::unique_ptr<Node> max;

	inline NodeMax(std::unique_ptr<Node> value, std::unique_ptr<Node> max) :
			value(std::move(value)),
			max(std::move(max)) {}

	virtual float evaluate(float x, float y) noexcept override;
};

struct NodeSin : Node {
	std::unique_ptr<Node> input;

	explicit inline NodeSin(std::unique_ptr<Node> input) :
			input(std::move(input)) {}

	virtual float evaluate(float x, float y) noexcept override;
};

struct NodeSmoothstep : Node {
	std::unique_ptr<Node> value;
	std::unique_ptr<Node> edge0;
	std::unique_ptr<Node> edge1;

	inline NodeSmoothstep(std::unique_ptr<Node> value, std::unique_ptr<Node> edge0, std::unique_ptr<Node> edge1) :
			value(std::move(value)),
			edge0(std::move(edge0)),
			edge1(std::move(edge1)) {}

	virtual float evaluate(float x, float y) noexcept override;
};

// -------------------------------------------------------------------------------------------------

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
