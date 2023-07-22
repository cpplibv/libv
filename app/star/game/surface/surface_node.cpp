// Project: libv, File: app/surface/surface/node.cpp

// hpp
#include <star/game/surface/surface_node.hpp>
// libv
#include <libv/math/mix.hpp>
#include <libv/math/step.hpp>
#include <libv/utility/approx.hpp>
// std
#include <cmath>
#include <functional> // For clamp


namespace star {

// -------------------------------------------------------------------------------------------------

// --- NodeConstant --------------------------------------------------------------------------------

float NodeConstant::evaluate(float x, float y) noexcept {
	(void) x;
	(void) y;
	return value;
}

std::string star::SurfaceNode::glsl(star::SurfaceNodeContext ctx) const noexcept {
	return {};
}

// --- NodeValue -----------------------------------------------------------------------------------

float NodeValue::evaluate(float x, float y) noexcept {
	return libv::noise::value(seed, x, y);
}

// --- NodePerlin ----------------------------------------------------------------------------------

float NodePerlin::evaluate(float x, float y) noexcept {
	return libv::noise::perlin(seed, x, y);
}

// --- NodeSimplex ---------------------------------------------------------------------------------


float NodeSimplex::evaluate(float x, float y) noexcept {
	return libv::noise::simplex(seed, x, y);
}

//	if (seed == 0)
//	return libv::noise_simplex2S(seed, x, y);
//	if (seed == 1)
//	return libv::noise_simplex(seed, x, y);
//	if (seed == 2)
//	return libv::noise_simplex_glsl(x, y);

// --- NodeCellular --------------------------------------------------------------------------------


float NodeCellular::evaluate(float x, float y) noexcept {
	return libv::noise::cellular(seed, x, y, distanceFn, returnType, jitter);
}

// --- NodeSimplexFractal ---------------------------------------------------------------------------

float NodeSimplexFractal::evaluate(float x, float y) noexcept {
	return libv::noise::fractal(seed, x, y, libv::noise::simplex, octaves, amplitude, frequency, lacunarity, persistence);
}

// --- NodeFractal --------------------------------------------------------------------------------------------

float NodeFractal::evaluate(float x, float y) noexcept {
	const Seed not_used_seed = 0;
	return libv::noise::fractal(not_used_seed, x, y, [&](Seed, float x0, float y0) {
				return input->evaluate(x0, y0);
			},
			octaves, amplitude, frequency, lacunarity, persistence);
}

// --- NodeSimplexGradientFractal --------------------------------------------------------------------------------------------

float NodeSimplexGradientFractal::evaluate(float x, float y) noexcept {
	auto [wx, wy] = libv::noise::fractal(seed, x, y, libv::noise::simplex_grad, octaves, amplitude, frequency, lacunarity, persistence);
	return input->evaluate(wx, wy);
}

// --- NodeCoordinate --------------------------------------------------------------------------------------------

float NodeCoordinate::evaluate(float x, float y) noexcept {
	return weights.x * x + weights.y * y;
}

// --- NodePlus --------------------------------------------------------------------------------------------

float NodePlus::evaluate(float x, float y) noexcept {
	float result = 0.f;
	for (const auto& input : inputs) {
		result += input->evaluate(x, y);
	}
	return result;
}

// --- NodeMul --------------------------------------------------------------------------------------------

float NodeMul::evaluate(float x, float y) noexcept {
	float result = 1.f;
	for (const auto& input : inputs) {
		result *= input->evaluate(x, y);
	}
	return result;
}

// --- NodePow --------------------------------------------------------------------------------------------

float NodePow::evaluate(float x, float y) noexcept {
	auto base = input->evaluate(x, y);
	if (base < 0)
		return -std::pow(-base, exponent);
	else
		return std::pow(base, exponent);
}


// --- NodeMix --------------------------------------------------------------------------------------------

float NodeMix::evaluate(float x, float y) noexcept {
	const auto t_ = t->evaluate(x, y);
	if (libv::approx(t_) == 0.f)
		return a->evaluate(x, y);
	else if (libv::approx(t_) == 1.f)
		return b->evaluate(x, y);
	else
		return libv::mix(a->evaluate(x, y), b->evaluate(x, y), t_);
}

// --- NodeClamp --------------------------------------------------------------------------------------------

float NodeClamp::evaluate(float x, float y) noexcept {
	const auto value_ = value->evaluate(x, y);
	const auto low_ = low->evaluate(x, y);
	const auto high_ = high->evaluate(x, y);

	return std::clamp(value_, low_, high_);
}

// --- NodeSaturate --------------------------------------------------------------------------------------------

float NodeSaturate::evaluate(float x, float y) noexcept {
	return std::clamp(input->evaluate(x, y), 0.f, 1.f);
}

// --- NodeMin --------------------------------------------------------------------------------------------

float NodeMin::evaluate(float x, float y) noexcept {
	return std::min(value->evaluate(x, y), min->evaluate(x, y));
}

// --- NodeMax --------------------------------------------------------------------------------------------

float NodeMax::evaluate(float x, float y) noexcept {
	return std::max(value->evaluate(x, y), max->evaluate(x, y));
}

// --- NodeSin --------------------------------------------------------------------------------------------

float NodeSin::evaluate(float x, float y) noexcept {
	return std::sin(input->evaluate(x, y));
}

// --- NodeSmoothstep --------------------------------------------------------------------------------------------

float NodeSmoothstep::evaluate(float x, float y) noexcept {
	const auto value_ = value->evaluate(x, y);
	const auto edge0_ = edge0->evaluate(x, y);
	const auto edge1_ = edge1->evaluate(x, y);

	return libv::smoothstep(value_, edge0_, edge1_);
}

// -------------------------------------------------------------------------------------------------



} // namespace star
