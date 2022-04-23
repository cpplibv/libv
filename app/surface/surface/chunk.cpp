// Created by dbobula on 1/16/2022.

#include <surface/surface/chunk.hpp>

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
#include <surface/surface/biome.hpp>


namespace surface {
// -------------------------------------------------------------------------------------------------

bool isPointInTriangle(libv::vec2f p, float step) {
	return libv::fract(p.x / step) + libv::fract(p.y / step) < 1;
}

// -------------------------------------------------------------------------------------------------

Chunk::Chunk(libv::vec2i index, libv::vec2f position, libv::vec2f size, uint32_t resolution) :
		index(index),
		position(position),
		size(size),
		resolution(resolution),
		rng(static_cast<uint64_t>(index.x) + (static_cast<uint64_t>(index.y) << 32u)),
		biomeMap(resolution, resolution),
		height(resolution, resolution),
		temperature(resolution, resolution),
		humidity(resolution, resolution),
		fertility(resolution, resolution),
		temp_humidity_distribution(resolution, resolution) {
}

//collusion query
float Chunk::getHeight(libv::vec2f query_position) const {
	//	             (1,1)
	// O    NW O  upY  O NE
	//       leftX  * rightX
	//
	// O    SW O downY O SE
	// |  \    |
	// |    \  |
	// O - - - O       O
	// (0,0)

	query_position /= size; // Remap query to the [0..1] quad

	const auto numQuad = resolution - 1;
	const auto step = 1.f / static_cast<float>(numQuad);

	const auto leftX = static_cast<int>(std::floor(query_position.x / step));
	const auto rightX = leftX + 1;
	const auto downY = static_cast<int>(std::floor(query_position.y / step));
	const auto upY = downY + 1;

	const auto NW = height(leftX, upY).pos;
	const auto NE = height(rightX, upY).pos;
	const auto SW = height(leftX, downY).pos;
	const auto SE = height(rightX, downY).pos;
	//triangle 1 = NW, SW, SE, triangle 2 = NW, SE, NE
	const auto isPointInNWSWSE = isPointInTriangle(query_position, step);

	auto u = libv::fract(query_position.x / step);
	auto v = libv::fract(query_position.y / step);

	if (isPointInNWSWSE)
		return (1 - u - v) * SW.z + u * SE.z + v * NW.z;
	else
		return (-1 + u + v) * NE.z + (1 - u) * NW.z + (1 - v) * SE.z;
}

//std::vector<libv::vec4f> Chunk::getColors(const libv::vector_2D<SurfacePoint>& points_) {
//	std::vector<libv::vec4f> colors;
//	colors.reserve(points_.size_x() * points_.size_y());
//	for (size_t y = 0; y < points_.size_y(); ++y) {
//		for (size_t x = 0; x < points_.size_x(); ++x) {
//			colors.emplace_back(points_(x, y).color);
//		}
//	}
//	return colors;
//}

// -------------------------------------------------------------------------------------------------

ChunkGen::ChunkGen() {}

void ChunkGen::placeVegetation(Chunk& chunk, const Config& config) {
	if (config.plantDistribution == PlantDistribution::random) {
		placeVegetationRandom(chunk, config);
	} else if (config.plantDistribution == PlantDistribution::clustered) {
//		chunk.placeVegetationClustered(config);
		throw std::runtime_error("Clustered plant distribution type not yet supported");
	} else
		throw std::runtime_error("Unknown plant distribution type");
}

//generalunk cluster centereket radiussal, aztan pedig ezekben a cluster korokben
// generalunk random pontokat az interpolacioval
//
//void Chunk::placeVegetationClustered(Chunk& chunk, const Config& config) {
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
//}

void ChunkGen::placeVegetationRandom(Chunk& chunk, const Config& config) {

	std::mutex chunkGuard;
	std::mutex rngGuard;
	chunk.veggies.reserve(config.numVeggie); // TODO P3: Should check if reserving for 20% isn't memory wasteful

	// NOTE: This parallelism doesn't yield too much gain, but still better than single thread
	// The entire generation of 81 chunk with 100 veggie per chunk in milliseconds:
	// Baseline / No operation: 145 - 148
	// Single thread: 170 - 180
	// Multi-thread: 159 - 162 (current)
	// Multi-thread lockless assigment: 158 - 161 (would need post merge, possible)

	libv::mt::parallel_for(threads, 0uz, config.numVeggie, [&](auto) {
		auto distX = libv::make_uniform_distribution_exclusive(0.f, chunk.size.x);
		auto distY = libv::make_uniform_distribution_exclusive(0.f, chunk.size.y);

		auto rngLock = std::unique_lock(rngGuard);
		auto rngLocal = rng.fork(); // RNG has to be forked under a mutex
		rngLock.unlock();

		const auto x = distX(rngLocal);
		const auto y = distY(rngLocal);
		const auto z = chunk.getHeight({x, y});

		const auto temp = config.temperature.rootNode->evaluate(x + chunk.position.x, y + chunk.position.y) - z * config.temperature.heightSensitivity;
		const auto wet = config.humidity.rootNode->evaluate(x + chunk.position.x, y + chunk.position.y);
//		const auto fert = config.fertility.rootNode->evaluate(x + position.x, y + position.y);

		auto picker = BiomePicker();
		auto mix = picker.mix(config.biomes, libv::vec2f(temp, wet));
		auto biome = mix.random(rngLocal);

		if(auto veggieType = mix.getRandomVeggieType(biome, rngLocal)) { //TODO: This should be veggie
			veggieType->pos = libv::vec3f{x + chunk.position.x, y + chunk.position.y, z}; //TODO: TAKE THIS OUT

			auto lock = std::unique_lock(chunkGuard);
			chunk.veggies.emplace_back(std::move(veggieType.value()));
		}
	});
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
	const auto chunkSize = libv::vec2f{2, 2};
	const auto chunkPosition = chunkIndex.cast<float>() * chunkSize;
	const auto numQuad = config.resolution;
	const auto numVertex = numQuad + 1;
	const auto step = chunkSize / static_cast<float>(numQuad);
//	const auto step = chunkSize / static_cast<float>(chunkResolution);

	Chunk chunk = Chunk(chunkIndex, chunkPosition, chunkSize, static_cast<uint32_t>(numVertex));

	const auto calc = [chunkPosition](const auto& node, const auto& colorGrad, const float x, const float y) {
		const auto noise_value = node->evaluate(x + chunkPosition.x, y + chunkPosition.y);
		const auto point = libv::vec3f{x, y, noise_value};
		const auto color = colorGrad.sample(noise_value);
		return SurfacePoint{point + libv::vec3f{chunkPosition, 0}, color};
	};

	chunk.temp_humidity_distribution.fill(0.f);

	libv::mt::parallel_for(threads, 0uz, numVertex, [&](auto yi) {
		const auto yf = static_cast<float>(yi);

		for (std::size_t xi = 0; xi < numVertex; ++xi) {
			const auto xf = static_cast<float>(xi);
			const auto x = xf * step.x;
			const auto y = yf * step.y;

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
