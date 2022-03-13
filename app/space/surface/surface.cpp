// Created by dbobula on 1/16/2022.

#include <space/surface/surface.hpp>
//timer
#include <libv/noise/noise.hpp>
#include <iostream>
#include <memory>
//#include <libv/math/smoothstep.hpp>


namespace surface {

//	ChunkGen::ChunkGen(space::Renderer& renderer):renderer(renderer) {
ChunkGen::ChunkGen() {
	range = libv::xoroshiro128(+123);
}

void ChunkGen::placeVegetation(Chunk& chunk, const Config& config) {
	if (config.plantDistribution == PlantDistribution::random) {
		placeVegetationRandom(chunk, config);
	} else if (config.plantDistribution == PlantDistribution::clustered) {
		placeVegetationClustered(chunk, config);
	} else throw std::runtime_error("Unknown plant distribution type");
}

template <typename DistFn>
libv::vec3f interpolateNeighbours(size_t x, size_t y, DistFn&& ratio, Chunk& chunk, libv::xoroshiro128& range) {

	const auto r1 = ratio(range);
	const auto r2 = ratio(range);
	const auto r3 = ratio(range);
	const auto point1 = chunk.points[x][y];
	const auto point2 = chunk.points[x][y + 1];
	const auto point3 = chunk.points[x + 1][y];
	const auto sum = r1 + r2 + r3;
	const auto r1_f = static_cast<float> (r1) / static_cast<float> (sum);
	const auto r2_f = static_cast<float> (r2) / static_cast<float> (sum);
	const auto r3_f = static_cast<float> (r3) / static_cast<float> (sum);
//		libv::smoothstep();
	const auto point = point1.point * r1_f + point2.point * r2_f + point3.point * r3_f;
	return point;
}

void ChunkGen::placeVegetationRandom(Chunk& chunk, const Config& config) {
	const auto size = config.size;
//	auto dist = libv::make_uniform_distribution_inclusive(1, config.treeNumber);
	auto chunkSize = libv::make_uniform_distribution_exclusive(0, size - 1);
//	auto veggie = libv::make_uniform_distribution_inclusive(0, 3);
	auto ratio = libv::make_uniform_distribution_inclusive(0.f, 1.f);
//	const auto treeCount = dist(range);
	for (const auto& object : config.objects) {
		SurfaceObjectStorage surfaceObjectStorage;
		surfaceObjectStorage.type = object.type;
		surfaceObjectStorage.points.reserve(object.count);
		for (int i = 0; i < object.count; ++i) {
			const auto x = chunkSize(range);
			const auto y = chunkSize(range);
			const auto point = interpolateNeighbours(x, y, ratio, chunk, range);
			surfaceObjectStorage.points.emplace_back(point, object.size, object.color);
//			chunk.featureList.emplace_back(VegetationPoint{point});
//				VegetationPoint{libv::vec3f(static_cast<float>(x) / size, static_cast<float>(y) / size, point.z)});
		}
		chunk.featureList.emplace_back(surfaceObjectStorage);
	}


//	for (int i = 0; i < size; ++i) {
//		const auto x = dist(range);
//		for (int j = 0; j < size; ++j) {
//			const auto y = dist(range);
//			chunk.vegetation.emplace_back(
//					VegetationPoint{libv::vec3f{static_cast<float>(x)/size,static_cast<float>(y)/size,0}});
//		}
//	}
//	bool hasTree = dist(range) == 1;
//			if (hasTree)
//				chunk.vegetation.emplace_back(VegetationPoint{point});
}

//generalunk cluster centereket radiussal, aztan pedig ezekben a cluster korokben
// generalunk random pontokat az interpolacioval

void ChunkGen::placeVegetationClustered(Chunk& chunk, const Config& config) {
	auto chunkSize = libv::make_uniform_distribution_exclusive(0, config.size - 1);
	auto ratio = libv::make_uniform_distribution_inclusive(0.f, 1.f);
//	float sum = 0.f;
	auto circleInterval = libv::make_uniform_distribution_inclusive(1, config.circleNumber);
	const auto circleCount = circleInterval(range);
//	for (int i = 0; i < circleCount; ++i) {
//		const auto radiusMax = 0.05f;
////		const auto circlePos = ;
//		auto treeRange = libv::make_uniform_distribution_inclusive(1, config.treeNumber);
//		auto radiusRange = libv::make_uniform_distribution_inclusive(1, config.treeNumber);
//
//		const auto radius = radiusRange(range);
//		const auto treeCount = treeRange(range);
//		for (int j = 0; j < treeCount; ++j) {
//
////			const auto point = interpolateNeighbours(x, y, ratio, chunk, range);
////			const auto point = interpolateNeighbours(chunkSize, ratio, chunk, range);
////			chunk.vegetation.emplace_back(VegetationPoint{point});
//		}
//	}
}


Chunk ChunkGen::generateChunk(const Config& config) {
	Chunk chunk;
	auto size = config.size;
	chunk.size = config.size;
	std::cout << "chunk.size: " << chunk.size << std::endl;
	chunk.points.resize(size);


	libv::mt::parallel_for(threads, 0, size, [&](auto i) {
//				libv::xoroshiro128 rng(seed + i);
//				auto dist = libv::make_uniform_distribution_inclusive(-1.f, 1.f);
		std::vector<SurfacePoint> temp;
		temp.resize(size);

		for (int j = 0; j < size; ++j) {
			auto i_f = static_cast<float>(i);
			auto j_f = static_cast<float>(j);
			auto size_f = static_cast<float>(size);

			const auto noise_value = config.rootNode->evaluate(i_f, j_f);
			const auto point = libv::vec3f{i_f / size_f, j_f / size_f, noise_value * 0.1f};
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
			temp[j] = SurfacePoint{point, color};
//					chunk.points[i][j] = SurfacePoint{libv::vec3f{i_f, j_f, noise_value * 0.1f}, color};
		}
		chunk.points[i] = std::move(temp);
	});
	return chunk;

}

} // namespace surface
