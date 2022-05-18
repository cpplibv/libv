// Created by dbobula on 3/1/2022.

// hpp
#include <surface/surface/node.hpp>
// libv
#include <libv/math/mix.hpp>
#include <libv/math/step.hpp>
#include <libv/utility/approx.hpp>
// std
#include <cmath>


namespace surface {

// -------------------------------------------------------------------------------------------------

float NodeConstant::evaluate(float x, float y) noexcept {
	(void) x;
	(void) y;
	return value;
}

float NodeValue::evaluate(float x, float y) noexcept {
	return libv::noise_value(seed, x, y);
}

float NodePerlin::evaluate(float x, float y) noexcept {
	return libv::noise_perlin(seed, x, y);
}

float NodeSimplex::evaluate(float x, float y) noexcept {
	return libv::noise_simplex(seed, x, y);

//	if (seed == 0)
//	return libv::noise_simplex2S(seed, x, y);
//	if (seed == 1)
//	return libv::noise_simplex(seed, x, y);
//	if (seed == 2)
//	return libv::noise_simplex_glsl(x, y);
}

float NodeCellular::evaluate(float x, float y) noexcept {
	return libv::noise_cellular(seed, x, y, distanceFn, returnType, jitter);
}

float NodeSimplexFractal::evaluate(float x, float y) noexcept {
	return libv::noise_fractal(seed, x, y, libv::noise_simplex, octaves, amplitude, frequency, lacunarity, persistence);
}

float NodeFractal::evaluate(float x, float y) noexcept {
	Seed not_used_seed = 0;
	return libv::noise_fractal(not_used_seed, x, y, [&](Seed, float x0, float y0) {
				return input->evaluate(x0, y0);
			},
			octaves, amplitude, frequency, lacunarity, persistence);
}

float NodeWarp::evaluate(float x, float y) noexcept {
	auto [wx, wy] = libv::warpFractalIndependent(seed, x, y, octaves, amplitude, frequency, lacunarity, persistence);
	return input->evaluate(wx, wy);
}

float NodeCoordinate::evaluate(float x, float y) noexcept {
	return weights.x * x + weights.y * y;
}

float NodePlus::evaluate(float x, float y) noexcept {
	float result = 0.f;
	for (const auto& input : inputs) {
		result += input->evaluate(x, y);
	}
	return result;
}

float NodeMul::evaluate(float x, float y) noexcept {
	float result = 1.f;
	for (const auto& input : inputs) {
		result *= input->evaluate(x, y);
	}
	return result;
}

float NodePow::evaluate(float x, float y) noexcept {
	auto base = input->evaluate(x, y);
	if (base < 0)
		return -std::pow(-base, exponent);
	else
		return std::pow(base, exponent);
}

float NodeMix::evaluate(float x, float y) noexcept {
	const auto t_ = t->evaluate(x, y);
	if (libv::approx(t_) == 0.f)
		return a->evaluate(x, y);
	else if (libv::approx(t_) == 1.f)
		return b->evaluate(x, y);
	else
		return libv::mix(a->evaluate(x, y), b->evaluate(x, y), t_);
}

float NodeClamp::evaluate(float x, float y) noexcept {
	const auto value_ = value->evaluate(x, y);
	const auto low_ = low->evaluate(x, y);
	const auto high_ = high->evaluate(x, y);

	return std::clamp(value_, low_, high_);
}

float NodeSaturate::evaluate(float x, float y) noexcept {
	return std::clamp(input->evaluate(x, y), 0.f, 1.f);
}

float NodeMin::evaluate(float x, float y) noexcept {
	return std::min(value->evaluate(x, y), min->evaluate(x, y));
}

float NodeMax::evaluate(float x, float y) noexcept {
	return std::max(value->evaluate(x, y), max->evaluate(x, y));
}

float NodeSin::evaluate(float x, float y) noexcept {
	return std::sin(input->evaluate(x, y));
}

float NodeSmoothstep::evaluate(float x, float y) noexcept {
	const auto value_ = value->evaluate(x, y);
	const auto edge0_ = edge0->evaluate(x, y);
	const auto edge1_ = edge1->evaluate(x, y);

	return libv::smoothstep(value_, edge0_, edge1_);
}

// -------------------------------------------------------------------------------------------------

} // namespace surface
