// Created by dbobula on 1/16/2022.

// hpp
#include <surface/surface/chunk.hpp>
// libv
#include <libv/math/fract.hpp>
#include <libv/utility/random/uniform_distribution.hpp>
#include <libv/utility/random/xoroshiro128.hpp>
// pro
#include <surface/surface/biome.hpp>
#include <surface/surface/node.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

/// @param p - point to test. Point has to be in the [0..1] unit square
[[nodiscard]] constexpr inline bool isPointInSWTriangle(libv::vec2f p) noexcept {
	return p.x + p.y < 1;
}

// -------------------------------------------------------------------------------------------------

Chunk::Chunk(libv::vec2i index, libv::vec2f position, libv::vec2f size, uint32_t resolution, Seed globalSeed) :
		index(index),
		position(position),
		size(size),
		resolution(resolution),
		rng(static_cast<uint64_t>(index.x) + (static_cast<uint64_t>(index.y) << 32u), globalSeed),
		height(resolution, resolution, libv::uninitialized),
		normal(resolution, resolution, libv::uninitialized),
		color(resolution, resolution, libv::uninitialized),
		temperature(resolution, resolution),
		humidity(resolution, resolution),
		fertility(resolution, resolution) {
//		temp_humidity_distribution(resolution, resolution) {
}

float Chunk::getInterpolatedHeight(libv::vec2f uv) const {
	//	             (1,1)
	// O    NW O  upY  O NE
	//       leftX  * rightX
	//
	// O    SW O downY O SE
	// |  \    |
	// |    \  |
	// O - - - O       O
	// (0,0)

	const auto numQuad = resolution - 1;
	const auto step = static_cast<float>(numQuad);

	const auto leftX = static_cast<int>(uv.x * step);
	const auto rightX = leftX + 1;
	const auto downY = static_cast<int>(uv.y * step);
	const auto upY = downY + 1;

	const auto NW = height(leftX, upY);
	const auto NE = height(rightX, upY);
	const auto SW = height(leftX, downY);
	const auto SE = height(rightX, downY);
	//triangle 1 = NW, SW, SE, triangle 2 = NW, SE, NE
	const auto quadU = libv::fract(uv.x * step);
	const auto quadV = libv::fract(uv.y * step);

	const auto isPointInNWSWSE = isPointInSWTriangle({quadU, quadV});

	if (isPointInNWSWSE)
		return (1 - quadU - quadV) * SW.z + quadU * SE.z + quadV * NW.z;
	else
		return (-1 + quadU + quadV) * NE.z + (1 - quadU) * NW.z + (1 - quadV) * SE.z;
}

libv::vec3f Chunk::pickRandomPoint(libv::xoroshiro128& rng) const {
	auto distUV = libv::make_uniform_distribution_exclusive(0.f, 1.f);
	const auto u = distUV(rng);
	const auto v = distUV(rng);

	const auto z = getInterpolatedHeight({u, v});
	return {position.x + (u - 0.5f) * size.x, position.y + (v - 0.5f) * size.y, z};
}

// -------------------------------------------------------------------------------------------------

ChunkGen::ChunkGen() {}

void ChunkGen::placeVegetation(const Config& config, Chunk& chunk) {
	if (config.plantDistribution == PlantDistribution::random) {
		placeVegetationRandom(config, chunk);
	} else if (config.plantDistribution == PlantDistribution::clustered) {
//		chunk.placeVegetationClustered(config);
		throw std::runtime_error("Clustered plant distribution type not yet supported");
	} else
		throw std::runtime_error("Unknown plant distribution type");
}

//generalunk cluster centereket radiussal, aztan pedig ezekben a cluster korokben
// generalunk random pontokat az interpolacioval
//
//void Chunk::placeVegetationClustered(const Config& config, Chunk& chunk) {
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

void ChunkGen::placeVegetationRandom(const Config& config, Chunk& chunk) {

	std::mutex chunk_m;
	std::mutex rng_m;
	chunk.veggies.reserve(config.numVeggie / 5); // TODO P3: Should check if reserving for 20% isn't memory wasteful | reserve more maybe even 100%, shrink to fit when done = 2 allocation, but never more, and 0 memory waste

	// NOTE: This parallelism doesn't yield too much gain, but still better than single thread
	// The entire generation of 81 chunk with 100 veggie per chunk in milliseconds:
	// Baseline / No operation: 145 - 148
	// Single thread: 170 - 180
	// Multi-thread: 159 - 162 (current)
	// Multi-thread lockless assigment: 158 - 161 (would need post merge, possible, not enough gain)

	libv::mt::parallel_for(threads, 0uz, config.numVeggie, [&](auto) {
		auto rngLock = std::unique_lock(rng_m);
		auto rngLocal = chunk.rng.fork(); // RNG has to be forked under a mutex
		rngLock.unlock();

		const auto point = chunk.pickRandomPoint(rngLocal);

		const auto temp = config.temperature.rootNode->evaluate(point.x, point.y) - point.z * config.temperature.heightSensitivity;
		const auto wet = config.humidity.rootNode->evaluate(point.x, point.y);
//		const auto fert = config.fertility.rootNode->evaluate(x + position.x, y + position.y);

		auto picker = BiomePicker();
		auto mix = picker.mix(config.biomes, libv::vec2f(temp, wet));

		const auto& biome = config.blendBiomes ? mix.random(rngLocal) : mix.primary();

		if (auto veggie = mix.getRandomVeggie(biome, rngLocal)) {
			veggie->pos = point;

			auto lock = std::unique_lock(chunk_m);
			chunk.veggies.emplace_back(std::move(veggie.value()));
		}
	});
}

//void ChunkGen::placeVegetationRandom(const Config& config, Chunk& chunk) {
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

void ChunkGen::generateChunk(const Config& config, Chunk& chunk) {
//	const auto chunkSize = libv::vec2f{8, 8};
//	const auto chunkPosition = chunkIndex.cast<float>() * chunkSize;
	const auto numQuad = config.resolution;
	const auto numVertex = numQuad + 1;
	const auto step = chunk.size / static_cast<float>(numQuad);
//	const auto step = chunkSize / static_cast<float>(chunkResolution);

//	chunk.temp_humidity_distribution.fill(0.f);

	libv::mt::parallel_for(threads, 0uz, numVertex, [&](auto yi) {
		const auto yf = static_cast<float>(yi);

		for (std::size_t xi = 0; xi < numVertex; ++xi) {
			const auto xf = static_cast<float>(xi);
			const auto x = xf * step.x - chunk.size.x * 0.5f;
			const auto y = yf * step.y - chunk.size.y * 0.5f;

			/// Calculate heatmaps' point
//			chunk.height(xi, yi) = calc(config.height.rootNode, config.height.colorGrad, x, y);
//									libv::vec3f{chunk.position.x + x, chunk.position.y + y, noise_value}
			chunk.height(xi, yi) = libv::vec3f{
					chunk.position.x + x,
					chunk.position.y + y,
					config.height.rootNode->evaluate(x + chunk.position.x, y + chunk.position.y)};
			chunk.temperature(xi, yi) = config.temperature.rootNode->evaluate(x + chunk.position.x, y + chunk.position.y);
			chunk.humidity(xi, yi) = config.humidity.rootNode->evaluate(x + chunk.position.x, y + chunk.position.y);

			chunk.fertility(xi, yi) = config.fertility.rootNode->evaluate(x + chunk.position.x, y + chunk.position.y);

//			chunk.temp_humidity_distribution(
//					std::clamp(static_cast<size_t>(chunk.humidity(xi, yi).pos.z * 127.f), 0uz, chunk.temp_humidity_distribution.size_x() - 1),
//					std::clamp(static_cast<size_t>(chunk.temperature(xi, yi).pos.z * 127.f), 0uz, chunk.temp_humidity_distribution.size_y() - 1)
//			) += 0.1f;

			const auto height = chunk.height(xi, yi).z;
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
//			const auto& biome = mix.random(rng);
//			auto weight = mix.primaryWeight();
//			const auto forest = categorizeForest(wet);
//			const auto fertility =
//					calculateFertility(0.25f, getMin(0.25f, temp),
//							0.33f, getMin(0.33f, wet),
//							height, temp, wet, fertilityOffset, config.temperature.heightSensitivity);
			//TODO: Dont calculate biomeMix for every vertex, just for n*n and then interpolate between them (like veggie points)
			if (config.blendBiomes) {
				chunk.color(xi, yi) = mix.blendedColor(1.0f);
			} else {
				const auto& biome = mix.primary();
				chunk.color(xi, yi) = biome.colorGrad.sample(1.0f);
			}

//			chunk.color(xi, yi) = libv::vec4f{weight, weight, weight, 1};
//			chunk.surface(xi, yi) = SurfacePoint{chunk.height(xi, yi).pos + libv::vec3f{chunk.position, 0},
//					{1,0,0,1}};
//			std::cout << " fertility: " << fertility<<std::endl;

		}
	});

	const auto calculateNormalFromPoints = [&](libv::vec3f pL, libv::vec3f pR, libv::vec3f pU, libv::vec3f pD) {
		return libv::normalize(libv::cross(pR - pL, pU - pD));
	};

	const auto calculateNormal = [&](std::size_t x, std::size_t y) {
		if (x == 0 || x == chunk.height.size_x() - 1)
			return libv::vec3f{0, 0, 1};
		if (y == 0 || y == chunk.height.size_y() - 1)
			return libv::vec3f{0, 0, 1};

		const auto pL = chunk.height(x - 1, y);
		const auto pR = chunk.height(x + 1, y);
		const auto pU = chunk.height(x, y + 1);
		const auto pD = chunk.height(x, y - 1);

		return calculateNormalFromPoints(pL, pR, pU, pD);
	};

//	threads.execute_and_wait()
//	libv::mt::parallel_for(threads, 0uz, numVertex, [&](auto yi) {
	libv::mt::parallel_for(threads, 1uz, numVertex - 1, [&](auto yi) {
//		const auto yf = static_cast<float>(yi);

		for (std::size_t xi = 1; xi < numVertex - 1; ++xi) {
//			const auto xf = static_cast<float>(xi);
//			const auto x = xf * step.x - chunk.size.x * 0.5f;
//			const auto y = yf * step.y - chunk.size.y * 0.5f;

//			chunk.height(xi, yi) = libv::vec3f{
//					chunk.position.x + x,
//					chunk.position.y + y,
//					config.height.rootNode->evaluate(x + chunk.position.x, y + chunk.position.y)};

			chunk.normal(xi, yi) = calculateNormal(xi, yi);
		}
	});

	libv::mt::parallel_for(threads, 0uz, 4uz, [&](auto sideIndex) {
		libv::vec2i start = libv::uninitialized;
		libv::vec2i direction = libv::uninitialized;
		libv::vec2f side = libv::uninitialized;

		switch (sideIndex) {
		default:
		case 0:
			start = {0, 0};
			direction = {1, 0};
			side = libv::vec2f{0, -1} * step;
			break;
		case 1:
			start = {static_cast<int32_t>(numVertex) - 1, 0};
			direction = {0, 1};
			side = libv::vec2f{1, 0} * step;
			break;
		case 2:
			start = {static_cast<int32_t>(numVertex) - 1, static_cast<int32_t>(numVertex) - 1};
			direction = {-1, 0};
			side = libv::vec2f{0, 1} * step;
			break;
		case 3:
			start = {0, static_cast<int32_t>(numVertex) - 1};
			direction = {0, -1};
			side = libv::vec2f{-1, 0} * step;
			break;
		}

		// !!! Corners
//		for (std::size_t i = 1; i < numVertex - 1; ++i) {

		for (std::size_t i = 1; i < numVertex - 1; ++i) {
			const auto pos = start + i * direction;
			const auto posf = pos.cast<float>();
			const auto x = posf.x * step.x - chunk.size.x * 0.5f;
			const auto y = posf.y * step.y - chunk.size.y * 0.5f;

//			const auto missingHeight = libv::vec3f{0, 0, 0};
			const auto missingHeight = libv::vec3f{
					chunk.position.x + x + side.x,
					chunk.position.y + y + side.y,
					config.height.rootNode->evaluate(x + chunk.position.x + side.x, y + chunk.position.y + side.y)};

			const auto pL = sideIndex == 3 ? missingHeight : chunk.height(pos.x - 1, pos.y);
			const auto pR = sideIndex == 1 ? missingHeight : chunk.height(pos.x + 1, pos.y);
			const auto pU = sideIndex == 2 ? missingHeight : chunk.height(pos.x, pos.y + 1);
			const auto pD = sideIndex == 0 ? missingHeight : chunk.height(pos.x, pos.y - 1);

			chunk.normal(pos) = calculateNormalFromPoints(pL, pR, pU, pD);
		}
	});
}

// -------------------------------------------------------------------------------------------------

} // namespace surface
