// Created by dbobula on 1/16/2022.

#include <space/surface/surface.hpp>
//timer
#include <libv/noise/noise.hpp>
#include <iostream>
#include <memory>
//libv
#include <libv/utility/approx.hpp>
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

[[nodiscard]] float fract(float x) {
	return x - std::floor(x);
}

bool isPointInTriangle(libv::vec2f p, float step) {
	return fract(p.x / step) + fract(p.y / step) < 1;
}

//collusion query
float getHeight(const libv::vec2f position, const Chunk& chunk) {
	//	             (1,1)
	// O    NW O  upY  O NE
	//       leftX  * rightX
	//
	// O    SW O downY O SE
	// |  \    |
	// |    \  |
	// O - - - O       O
	// (0,0)
	const auto numQuad = (chunk.points.size_x() - 1);
	const auto step = 1.f / numQuad;

	const auto leftX = static_cast<int>(std::floor(position.x / step));
	const auto rightX = leftX + 1;
	const auto downY = static_cast<int>(std::floor(position.y / step));
	const auto upY = downY + 1;

	const auto NW = chunk.points(leftX, upY).point;
	const auto NE = chunk.points(rightX, upY).point;
	const auto SW = chunk.points(leftX, downY).point;
	const auto SE = chunk.points(rightX, downY).point;
	//triangle 1 = NW, SW, SE, triangle 2 = NW, SE, NE
	const auto isPointInNWSWSE = isPointInTriangle(position, step);

	auto u = fract(position.x / step);
	auto v = fract(position.y / step);

	if (isPointInNWSWSE)
		return (1 - u - v) * SW.z + u * SE.z + v * NW.z;
	else
		return (-1 + u + v) * NE.z + (1 - u) * NW.z + (1 - v) * SE.z;
}

template <typename DistFn>
libv::vec3f interpolateNeighbours(float x, float y, DistFn&& ratio, Chunk& chunk, libv::xoroshiro128& range) {

	const auto r1 = ratio(range);
	const auto r2 = ratio(range);
	const auto r3 = ratio(range);
	const auto r4 = ratio(range);
	const auto point1 = chunk.points(x, y);
	const auto point2 = chunk.points(x, y + 1);
	const auto point3 = chunk.points(x + 1, y);
	const auto point4 = chunk.points(x + 1, y + 1);
	const auto sum = r1 + r2 + r3 + r4;
	const auto r1_f = static_cast<float> (r1) / static_cast<float> (sum);
	const auto r2_f = static_cast<float> (r2) / static_cast<float> (sum);
	const auto r3_f = static_cast<float> (r3) / static_cast<float> (sum);
	const auto r4_f = static_cast<float> (r4) / static_cast<float> (sum);
//		libv::smoothstep();
	const auto point = point1.point * r1_f + point2.point * r2_f + point3.point * r3_f + point4.point * r4_f;
	return point;
}

void ChunkGen::placeVegetationRandom(Chunk& chunk, const Config& config) {
	const auto numQuad = config.resolution;
	auto ratio = libv::make_uniform_distribution_exclusive(0.f, 1.f);
	for (const auto& object : config.objects) {
		SurfaceObjectStorage surfaceObjectStorage;
		surfaceObjectStorage.type = object.type;
		surfaceObjectStorage.points.reserve(object.count);
		for (int i = 0; i < object.count; ++i) {
			const auto x = ratio(range);
			const auto y = ratio(range);
			const auto z = getHeight({x, y}, chunk);
			surfaceObjectStorage.points.emplace_back(libv::vec3f{x + chunk.position.x, y + chunk.position.y, z}, object.size, object.color);
		}
		chunk.featureList.emplace_back(surfaceObjectStorage);
	}
}

//generalunk cluster centereket radiussal, aztan pedig ezekben a cluster korokben
// generalunk random pontokat az interpolacioval

void ChunkGen::placeVegetationClustered(Chunk& chunk, const Config& config) {
	auto chunkSize = libv::make_uniform_distribution_exclusive(0, config.resolution - 1);
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

Chunk ChunkGen::generateChunk(const Config& config, const libv::vec2f chunkPosition) {
	Chunk chunk;
	chunk.position = chunkPosition;
	const auto numQuad = config.resolution;
	const auto numVertex = numQuad + 1;
	chunk.size = numQuad;
//	std::cout << "chunk.size: " << chunk.size << std::endl;
	chunk.points = {numVertex, numVertex};


	libv::mt::parallel_for(threads, size_t{0}, numVertex, [&](auto yi) {
		const auto yf = static_cast<float>(yi);
		const auto size_f = static_cast<float>(numQuad);
		for (int xi = 0; xi < numVertex; ++xi) {
			const auto xf = static_cast<float>(xi);
			const auto noise_value = config.rootNode->evaluate(xf / size_f + chunkPosition.x, yf / size_f + chunkPosition.y);
			const auto point = libv::vec3f{xf / size_f, yf / size_f, noise_value * 0.1f};
			const auto color = config.colorGrad.sample(noise_value);

			chunk.points(xi, yi) = SurfacePoint{point + libv::vec3f{chunkPosition, 0}, color};
		}
	});
	return chunk;
}

} // namespace surface
