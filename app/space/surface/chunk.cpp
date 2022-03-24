// Created by dbobula on 1/16/2022.

#include <space/surface/chunk.hpp>

//ext
#include <fmt/format.h>
//libv
#include <libv/noise/noise.hpp>
#include <libv/math/fract.hpp>
// std
#include <functional>
#include <iostream>
#include <memory>
//space
#include <space/surface/biome.hpp>


namespace surface {
// -------------------------------------------------------------------------------------------------

bool isPointInTriangle(libv::vec2f p, float step) {
	return libv::fract(p.x / step) + libv::fract(p.y / step) < 1;
}

// -------------------------------------------------------------------------------------------------

Chunk::Chunk(const size_t size_, const libv::vec2i index, const libv::vec2f position_) :
		size(size_),
		rng(static_cast<uint64_t>(index.x) + (static_cast<uint64_t>(index.y) << 32)),
		position(position_),
		biomeMap(size_, size_),
		height(size_, size_),
		temperature(size_, size_),
		humidity(size_, size_),
		fertility(size_, size_),
		temp_humidity_distribution(size_, size_) {
}

//collusion query
float Chunk::getHeight(const libv::vec2f position, const libv::vector_2D<SurfacePoint>& heatMap) {
	//	             (1,1)
	// O    NW O  upY  O NE
	//       leftX  * rightX
	//
	// O    SW O downY O SE
	// |  \    |
	// |    \  |
	// O - - - O       O
	// (0,0)
	const auto numQuad = (heatMap.size_x() - 1);
	const auto step = 1.f / static_cast<float>(numQuad);

	const auto leftX = static_cast<int>(std::floor(position.x / step));
	const auto rightX = leftX + 1;
	const auto downY = static_cast<int>(std::floor(position.y / step));
	const auto upY = downY + 1;

	const auto NW = heatMap(leftX, upY).pos;
	const auto NE = heatMap(rightX, upY).pos;
	const auto SW = heatMap(leftX, downY).pos;
	const auto SE = heatMap(rightX, downY).pos;
	//triangle 1 = NW, SW, SE, triangle 2 = NW, SE, NE
	const auto isPointInNWSWSE = isPointInTriangle(position, step);

	auto u = libv::fract(position.x / step);
	auto v = libv::fract(position.y / step);

	if (isPointInNWSWSE)
		return (1 - u - v) * SW.z + u * SE.z + v * NW.z;
	else
		return (-1 + u + v) * NE.z + (1 - u) * NW.z + (1 - v) * SE.z;
}

std::vector<libv::vec4f> Chunk::getColors(const libv::vector_2D<SurfacePoint>& points_) {
	std::vector<libv::vec4f> colors;
	colors.reserve(points_.size_x() * points_.size_y());
	for (size_t y = 0; y < points_.size_y(); ++y) {
		for (size_t x = 0; x < points_.size_x(); ++x) {
			colors.emplace_back(points_(x, y).color);
		}
	}
	return colors;
}

void Chunk::placeVegetationRandom(const Config& config) {
	auto ratio = libv::make_uniform_distribution_exclusive(0.f, 1.f);

	for (size_t i = 0; i < config.numVeggie; ++i) {
		const auto x = ratio(rng);
		const auto y = ratio(rng);
		const auto z = getHeight({x, y}, height);

		const auto temp = config.temperature.rootNode->evaluate(x + position.x, y + position.y) - z * config.temperature.heightSensitivity;
		const auto wet = config.humidity.rootNode->evaluate(x + position.x, y + position.y);
//		const auto fert = config.fertility.rootNode->evaluate(x + position.x, y + position.y);

		auto picker = BiomePicker();
		auto mix = picker.mix(config.biomes, libv::vec2f(temp, wet));
		auto biome = mix.random(rng);

		if(auto veggieType = mix.getRandomVeggieType(biome, rng)) { //TODO: This should be veggie
			veggieType->pos = libv::vec3f{x + position.x, y + position.y, z}; //TODO: TAKE THIS OUT
			veggies.emplace_back(veggieType.value());
		}
//		Veggie result;
//		result.

	}

}

//generalunk cluster centereket radiussal, aztan pedig ezekben a cluster korokben
// generalunk random pontokat az interpolacioval

void Chunk::placeVegetationClustered(const Config& config) {
	(void) config;
//	auto chunkSize = libv::make_uniform_distribution_exclusive(0, config.resolution - 1);
//	auto ratio = libv::make_uniform_distribution_inclusive(0.f, 1.f);
////	float sum = 0.f;
//	auto circleInterval = libv::make_uniform_distribution_inclusive(1, config.circleNumber);
//	const auto circleCount = circleInterval(rng);
//	for (int i = 0; i < circleCount; ++i) {
//		const auto radiusMax = 0.05f;
////		const auto circlePos = ;
//		auto treeRange = libv::make_uniform_distribution_inclusive(1, config.treeNumber);
//		auto radiusRange = libv::make_uniform_distribution_inclusive(1, config.treeNumber);
//
//		const auto radius = radiusRange(rng);
//		const auto treeCount = treeRange(rng);
//		for (int j = 0; j < treeCount; ++j) {
//
////			const auto point = interpolateNeighbours(x, y, ratio, chunk, rng);
////			const auto point = interpolateNeighbours(chunkSize, ratio, chunk, rng);
////			chunk.vegetation.emplace_back(VegetationPoint{point});
//		}
//	}
}

// -------------------------------------------------------------------------------------------------

ChunkGen::ChunkGen() {}

void ChunkGen::placeVegetation(Chunk& chunk, const Config& config) {
	if (config.plantDistribution == PlantDistribution::random) {
		chunk.placeVegetationRandom(config);
	} else if (config.plantDistribution == PlantDistribution::clustered) {
		chunk.placeVegetationClustered(config);
	} else
		throw std::runtime_error("Unknown plant distribution type");
}

//void ChunkGen::placeVegetationRandom(Chunk& chunk, const Config& config) {
//	const auto numQuad = config.resolution;
//	auto ratio = libv::make_uniform_distribution_exclusive(0.f, 1.f);
//
//	for (const auto& object : config.objects) {
//		SurfaceObjectStorage surfaceObjectStorage;
//		surfaceObjectStorage.type = object.type;
//		surfaceObjectStorage.points.reserve(object.count);
//
//		for (int i = 0; i < object.count; ++i) {
//			const auto x = ratio(rng);
//			const auto y = ratio(rng);
//			const auto z = chunk.getHeight({x, y});
//			surfaceObjectStorage.points.emplace_back(libv::vec3f{x + chunk.position.x, y + chunk.position.y, z}, object.size, object.color);
//		}
//		chunk.featureList.emplace_back(surfaceObjectStorage);
//	}
//}

//void Chunk::setTexturePoints(const Config& config) {
//	const auto numQuad = config.resolution;
//	const auto numVertex = numQuad + 1;
//
//	libv::mt::parallel_for(threads, size_t{0}, numVertex, [&](auto yi) {
//		const auto yf = static_cast<float>(yi);
//		const auto size_f = static_cast<float>(numQuad);
//
//		for (int xi = 0; xi < numVertex; ++xi) {
//			const auto xf = static_cast<float>(xi);
//			const auto noise_value = config.rootNode->evaluate(xf / size_f + position.x, yf / size_f + position.y);
//			const auto point = libv::vec3f{xf / size_f, yf / size_f, noise_value * 0.1f};
//			const auto color = config.colorGrad.sample(noise_value);
//
//			points(xi, yi) = SurfacePoint{point + libv::vec3f{position, 0}, color};
//		}
//	});
//}

Chunk ChunkGen::generateChunk(const Config& config, const libv::vec2i chunkIndex) {
	const auto chunkPosition = chunkIndex.cast<float>();
	const auto numQuad = config.resolution;
	const auto numVertex = numQuad + 1;
	Chunk chunk = Chunk(numVertex, chunkIndex, chunkPosition);

	const auto calc = [chunkPosition](const auto& node, const auto& colorGrad, const float x, const float y) {
		const auto noise_value = node->evaluate(x + chunkPosition.x, y + chunkPosition.y);
		const auto point = libv::vec3f{x, y, noise_value};
		const auto color = colorGrad.sample(noise_value);
		return SurfacePoint{point + libv::vec3f{chunkPosition, 0}, color};
	};

	chunk.temp_humidity_distribution.fill(0.f);

	libv::mt::parallel_for(threads, size_t{0}, numVertex, [&](auto yi) {
		const auto yf = static_cast<float>(yi);
		const auto size_f = static_cast<float>(numQuad);

		for (size_t xi = 0; xi < numVertex; ++xi) {
			const auto xf = static_cast<float>(xi);
			const auto x = xf / size_f;
			const auto y = yf / size_f;

			/// Calculate heatmaps' point
			chunk.height(xi, yi) = calc(config.height.rootNode, config.height.colorGrad, x, y);
			chunk.temperature(xi, yi) = config.temperature.rootNode->evaluate(x + chunkPosition.x, y + chunkPosition.y);
			chunk.humidity(xi, yi) = config.humidity.rootNode->evaluate(x + chunkPosition.x, y + chunkPosition.y);

			chunk.fertility(xi, yi) = config.fertility.rootNode->evaluate(x + chunkPosition.x, y + chunkPosition.y);

//			chunk.temp_humidity_distribution(
//					std::clamp(static_cast<size_t>(chunk.humidity(xi, yi).pos.z * 127.f), 0uz, chunk.temp_humidity_distribution.size_x() - 1),
//					std::clamp(static_cast<size_t>(chunk.temperature(xi, yi).pos.z * 127.f), 0uz, chunk.temp_humidity_distribution.size_y() - 1)
//			) += 0.1f;

			const auto height = chunk.height(xi, yi).pos.z;
			const auto temp = chunk.temperature(xi, yi) - height * config.temperature.heightSensitivity;
			const auto wet = chunk.humidity(xi, yi);
			const auto fertilityOffset = chunk.fertility(xi, yi);

			(void) fertilityOffset;
//			const auto biome = categorizeZone(temp);
//			const auto biomeIt = config.biomes.find(biome);
//			if (biomeIt == config.biomes.end())
//				throw std::runtime_error(fmt::format("Biome not found: {}", std::to_underlying(biome)));
			auto picker = BiomePicker();
			auto mix = picker.mix(config.biomes, libv::vec2f(temp, wet));
//			auto biome = mix.primary();
//			auto weight = mix.primaryWeight();
//			const auto forest = categorizeForest(wet);
//			const auto fertility =
//					calculateFertility(0.25f, getMin(0.25f, temp),
//							0.33f, getMin(0.33f, wet),
//							height, temp, wet, fertilityOffset, config.temperature.heightSensitivity);
			//TODO: Dont calculate biomeMix for every vertex, just for n*n and then interpolate between them (like veggie points)
			chunk.biomeMap(xi, yi) = mix.blendedColor(0.7f);

//			chunk.biomeMap(xi, yi) = libv::vec4f{weight, weight, weight, 1};
//			chunk.surface(xi, yi) = SurfacePoint{chunk.height(xi, yi).pos + libv::vec3f{chunkPosition, 0},
//					{1,0,0,1}};
//			std::cout << " fertility: " << fertility<<std::endl;

		}
	});
	return chunk;
}
// -------------------------------------------------------------------------------------------------

} // namespace surface