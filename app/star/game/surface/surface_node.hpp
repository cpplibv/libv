// Project: libv, File: app/surface/surface/node.hpp

#pragma once

// ext
#include <boost/container/small_vector.hpp>
// libv
#include <libv/math/vec.hpp>
#include <libv/noise/noise_2.hpp>
// std
#include <memory>
// pro
// #include <surface/surface/fwd.hpp>


namespace star {

using Seed = uint32_t;

// --- SurfaceNodeContext --------------------------------------------------------------------------

struct SurfaceNodeContext {

};

// --- SurfaceNode ---------------------------------------------------------------------------------

struct SurfaceNode {
	[[nodiscard]] virtual float evaluate(float x, float y) noexcept = 0;
	[[nodiscard]] virtual std::string glsl(SurfaceNodeContext ctx) const noexcept;
	virtual ~SurfaceNode() = default;
};

// -------------------------------------------------------------------------------------------------

struct NodeConstant : SurfaceNode {
	float value = 0.f;

	explicit NodeConstant(float value) : value(value) {};
	virtual float evaluate(float x, float y) noexcept override;
};

struct NodeValue : SurfaceNode {
	Seed seed = 0x5EED;

	explicit NodeValue(Seed seed) : seed(seed) {};
	virtual float evaluate(float x, float y) noexcept override;
};

struct NodePerlin : SurfaceNode {
	Seed seed = 0x5EED;

	explicit NodePerlin(Seed seed) : seed(seed) {};
	virtual float evaluate(float x, float y) noexcept override;
};

struct NodeSimplex : SurfaceNode {
	Seed seed = 0x5EED;

	explicit NodeSimplex(Seed seed) : seed(seed) {};
	virtual float evaluate(float x, float y) noexcept override;
};

struct NodeCellular : SurfaceNode {
	Seed seed = 0x5EED;

	libv::noise::CellularDistanceFunction distanceFn = libv::noise::CellularDistanceFunction::euclidean;
	libv::noise::CellularReturnType returnType = libv::noise::CellularReturnType::cellValue;
	float jitter = 1.0f;

	NodeCellular(Seed seed, libv::noise::CellularDistanceFunction distanceFn, libv::noise::CellularReturnType returnType, float jitter) :
			seed(seed), distanceFn(distanceFn), returnType(returnType), jitter(jitter) {}
	virtual float evaluate(float x, float y) noexcept override;
};

struct BaseFractalNode : SurfaceNode {
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
	std::unique_ptr<SurfaceNode> input;

	explicit inline NodeFractal(std::unique_ptr<SurfaceNode> input) :
			input(std::move(input)) {};
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

struct NodeSimplexGradientFractal : BaseFractalNode {
	std::unique_ptr<SurfaceNode> input;

	explicit inline NodeSimplexGradientFractal(std::unique_ptr<SurfaceNode> input) :
				input(std::move(input)) {};

	virtual float evaluate(float x, float y) noexcept override;

//	virtual int32_t glsl(ScriptGLSL& glsl, std::string_view coordinates) override {
//		const auto resultCoordID = glsl.nextCoordID();
//		glsl.outln("vec2 c{} = warpFractalIndependent({});", resultCoordID, coordinates);
//		return input->glsl(glsl, "c{}", resultCoordID);
//	}
};

//TODO fractal progressive, simplexgradient

struct NodeCoordinate : SurfaceNode {
	libv::vec2f weights;

	explicit NodeCoordinate(const libv::vec2f& weights) : weights(weights) {};
	virtual float evaluate(float x, float y) noexcept override;

//	virtual int32_t glsl(ScriptGLSL& glsl, std::string_view coordinates) override {
//		const auto resultID = glsl.nextVarID();
//		glsl.outln("float v{} = {}.x * {} + {}.y * {};", resultID, coordinates, weights.x, coordinates, weights.y);
//		return resultID;
//	}
};

struct NodePlus : SurfaceNode {
	using container = boost::container::small_vector<std::unique_ptr<SurfaceNode>, 4>;
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

struct NodeMul : SurfaceNode {
	using container = boost::container::small_vector<std::unique_ptr<SurfaceNode>, 4>;
	container inputs;

	explicit inline NodeMul(container inputs) :
			inputs(std::move(inputs)) {}

	virtual float evaluate(float x, float y) noexcept override;
};

struct NodePow : SurfaceNode {
	std::unique_ptr<SurfaceNode> input;
	float exponent = 1.f;

	explicit inline NodePow(std::unique_ptr<SurfaceNode> input, float exponent) :
			input(std::move(input)), exponent(exponent) {}

	virtual float evaluate(float x, float y) noexcept override;
};

struct NodeMix : SurfaceNode {
	std::unique_ptr<SurfaceNode> a;
	std::unique_ptr<SurfaceNode> b;
	std::unique_ptr<SurfaceNode> t;

	inline NodeMix(std::unique_ptr<SurfaceNode> a, std::unique_ptr<SurfaceNode> b, std::unique_ptr<SurfaceNode> t) :
			a(std::move(a)),
			b(std::move(b)),
			t(std::move(t)) {}

	virtual float evaluate(float x, float y) noexcept override;
};

struct NodeClamp : SurfaceNode {
	std::unique_ptr<SurfaceNode> value;
	std::unique_ptr<SurfaceNode> low;
	std::unique_ptr<SurfaceNode> high;

	inline NodeClamp(std::unique_ptr<SurfaceNode> value, std::unique_ptr<SurfaceNode> low, std::unique_ptr<SurfaceNode> high) :
			value(std::move(value)),
			low(std::move(low)),
			high(std::move(high)) {}

	virtual float evaluate(float x, float y) noexcept override;
};

struct NodeSaturate : SurfaceNode {
	std::unique_ptr<SurfaceNode> input;

	explicit inline NodeSaturate(std::unique_ptr<SurfaceNode> input) :
			input(std::move(input)) {}

	virtual float evaluate(float x, float y) noexcept override;
};

struct NodeMin : SurfaceNode {
	std::unique_ptr<SurfaceNode> value;
	std::unique_ptr<SurfaceNode> min;

	inline NodeMin(std::unique_ptr<SurfaceNode> value, std::unique_ptr<SurfaceNode> min) :
			value(std::move(value)),
			min(std::move(min)) {}

	virtual float evaluate(float x, float y) noexcept override;
};

struct NodeMax : SurfaceNode {
	std::unique_ptr<SurfaceNode> value;
	std::unique_ptr<SurfaceNode> max;

	inline NodeMax(std::unique_ptr<SurfaceNode> value, std::unique_ptr<SurfaceNode> max) :
			value(std::move(value)),
			max(std::move(max)) {}

	virtual float evaluate(float x, float y) noexcept override;
};

struct NodeSin : SurfaceNode {
	std::unique_ptr<SurfaceNode> input;

	explicit inline NodeSin(std::unique_ptr<SurfaceNode> input) :
			input(std::move(input)) {}

	virtual float evaluate(float x, float y) noexcept override;
};

struct NodeSmoothstep : SurfaceNode {
	std::unique_ptr<SurfaceNode> value;
	std::unique_ptr<SurfaceNode> edge0;
	std::unique_ptr<SurfaceNode> edge1;

	inline NodeSmoothstep(std::unique_ptr<SurfaceNode> value, std::unique_ptr<SurfaceNode> edge0, std::unique_ptr<SurfaceNode> edge1) :
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

} // namespace star
