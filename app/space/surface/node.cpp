// Created by dbobula on 3/1/2022.

#include <space/surface/node.hpp>


namespace surface {
float NodeConstant::evaluate(float x, float y) {
	return value;
}

float NodePlus::evaluate(float x, float y) {
	float result = 0.f;
	for (const auto& input : inputs) {
		result += input->evaluate(x, y);
	}
	return result;
}

float NodeMul::evaluate(float x, float y) {
	float result = 1.f;
	for (const auto& input : inputs) {
		result *= input->evaluate(x, y);
	}
	return result;
}

float NodePow::evaluate(float x, float y) {
	auto base = input->evaluate(x, y);
	if (base < 0)
		return -std::pow(-base, exponent);
	else
		return std::pow(base, exponent);
}

}
