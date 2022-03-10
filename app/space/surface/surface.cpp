// Created by dbobula on 1/16/2022.

#include <space/surface/surface.hpp>
//timer
#include <libv/noise/noise.hpp>
#include <iostream>
#include <memory>


namespace surface {

NoiseGen::NoiseGen() {

}

Surface NoiseGen::generateNoise(const Config& config) {
	Surface surface;
	auto size = config.size;
	surface.size = config.size;
	std::cout << "surface.size: " << surface.size << std::endl;
	surface.points.resize(size);

//	libv::gradientf<libv::vec4f> colorGrad;
//	colorGrad.add(-2.f, {0, 0, 1, 1});
//	colorGrad.add(-0.2f, {0, 0.4f, 1, 0.7f});
//	colorGrad.add(0.1f, {0.8f, 0.8f, 0.15f, 1});
////	colorGrad.add(0.f, {1, 1, 1, 1});
//	colorGrad.add(0.2f, {0.1f, 0.5f, 0.1f, 1});
//	colorGrad.add(0.5f, {0.1f, 0.3f, 0.1f, 1});
//	colorGrad.add(1.f, {0, 0, 0, 0.3f});

	libv::mt::parallel_for(threads, 0, size, [&](auto i) {
//				libv::xoroshiro128 rng(seed + i);
//				auto dist = libv::make_uniform_distribution_inclusive(-1.f, 1.f);
		std::vector<SurfacePoint> temp;
		temp.resize(size);

		for (int j = 0; j < size; ++j) {
			auto i_f = static_cast<float>(i);
			auto j_f = static_cast<float>(j);
			auto size_f = static_cast<float>(size);

			const auto noise_value = config.rootNode->evaluate(i_f , j_f);
			const auto color = config.colorGrad.sample(noise_value);
//			const auto color = libv::vec4f{libv::vec3f::one(noise_value / (config.amplitude * 2) * 0.5f + 0.5f), 1.f};

//			plus.inputs =
//			auto noise_value = libv::noise_fractal(config.seed, i_f / 50.f, j_f / 50.f, [](auto... a){ return libv::noise_cellular<float>(a...,
//					libv::CellularDistanceFunction::euclidean,
//					libv::CellularReturnType::cellValue
//				);}, config.octaves, config.amplitude, config.frequency, config.lacunarity, config.persistence);
//
//			noise_value += libv::noise_fractal(config.seed, i_f / 50.f, j_f / 50.f, libv::noise_simplex<float>, config.octaves, config.amplitude, config.frequency, config.lacunarity, config.persistence);

//					const auto color = libv::vec4f{libv::color::hue_to_rgb(1.f / 3.f * (noise_value * 0.5f + 0.5f)), 1.f};

//							libv::vec4f{1, 1, 1, 1}
//					renderer.debug.points.emplace_back(
//							libv::vec3f{i_f, j_f, noise_value*0.1f},
//							color
//					);
			temp[j] = SurfacePoint{libv::vec3f{i_f / size_f, j_f / size_f, noise_value * 0.1f}, color};
//					surface.points[i][j] = SurfacePoint{libv::vec3f{i_f, j_f, noise_value * 0.1f}, color};
		}
		surface.points[i] = std::move(temp);
	});
	return surface;

}

} // namespace surface
