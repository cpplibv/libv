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

float fract(float x){
	return x - std::floor(x);
}

//TODO simplify bc its |_ triangle
bool pointInTopLeftBottomRightTriangle(libv::vec2f p, float step) {
	// a-----
	//  |\  |
	//  | \ |
	// b|__>|c
//	const auto w = (b - c).length();
//	const auto h = (b - a).length();
//	const auto temp = w * p.y + h * p.x;

	return  fract(p.x/step) + fract(p.y/step) < 1;
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
	const auto numQuad = (chunk.points.size() - 1);
	const auto step = 1.f / numQuad;
	const auto leftX = static_cast<int>(std::floor(position.x / step));
	const auto rightX = leftX + 1;
	const auto downY = static_cast<int>(std::floor(position.y / step));
	const auto upY = downY + 1;
//	std::cout << position / step;
//	std::cout << " leftX:\t" << leftX;
//	std::cout << " rightX:\t" << rightX;
//	std::cout << " downY:\t" << downY;
//	std::cout << " upY:\t" << upY << std::endl;
//	const auto NW = chunk.points[upY][leftX].point;
//	const auto NE = chunk.points[upY][rightX].point;
//	const auto SW = chunk.points[downY][leftX].point;
//	const auto SE = chunk.points[downY][rightX].point;
	const auto NW = chunk.points[upY][leftX].point;
	const auto NE = chunk.points[upY][rightX].point;
	const auto SW = chunk.points[downY][leftX].point;
	const auto SE = chunk.points[downY][rightX].point;
	//triangle 1 = NW, SW, SE, triangle 2 = NW, SE, NE
	const auto isPointInNWSWSE = pointInTopLeftBottomRightTriangle(position, step);
//	const auto isPointInNWSENE = PointInTriangle(position, xy(NW), xy(SW), xy(SE));

//	const auto distNW = (position - xy(NW)).length();
//	const auto distSE = (position - xy(SE)).length();

	 auto u = fract(position.x / step);
	 auto v = fract(position.y / step);

//	return NW.z;
//	return NE.z;
//	return SE.z;
//	return SW.z;

//	const auto weightNW = 1.f / distNW;
//	const auto weightSE = 1.f / distSE;
//	return (weightNW * NW.z + weightSE * SE.z) / (weightNW + weightSE);

//	if (distNW == libv::approx(0.f))
//		return NW.z;
//	if (distSE == libv::approx(0.f))
//		return SE.z;

	if (isPointInNWSWSE) {
		return (1 - u - v) * SW.z + u * SE.z + v * NW.z;

//		const auto distSW = (position - xy(SW)).length();
////		if (distSW == libv::approx(0.f))
////			return SW.z;
//
//		const auto sum2 = 1.f / distNW + 1.f / distSW + 1.f / distSE;
//		const auto z = (NW.z * (1.f / distNW) + SW.z * (1.f / distSW) + SE.z * (1.f / distSE)) / sum2;
//		return z;

	} else {
		return (-1 + u + v) * NE.z + (1 - u) * NW.z + (1 - v) * SE.z;

//		return (1 - 1 + u - 1 + v) * NE.z + (1 - u) * NW.z + (1 - v) * SE.z;

//		u = 1 - u;
//		v = 1 - v;
//		return (1 - u - v) * NE.z + u * NW.z + v * SE.z;

//		return (1 - u - v) * NE.z + u * SE.z + v * NW.z;

//		return -5;

//		const auto distNE = (position - xy(NE)).length();
////		if (distNE == libv::approx(0.f))
////			return NE.z;
//
//		const auto sum2 = 1.f / distNW + 1.f / distSE + 1.f / distNE;
//		const auto z = (NW.z * (1.f / distNW) + SE.z * (1.f / distSE) + NE.z * (1.f / distNE)) / sum2;
//		return z;
	}

//	const auto min = std::min({distNW, distSW, distSE, distNE});
//	std::cout << "distNW: " << distNW << std::endl;
//	std::cout << "distSW: " << distSW << std::endl;
//	std::cout << "distSE: " << distSE << std::endl;
//	std::cout << "distNE: " << distNE << std::endl;
//	std::cout << "min: " << min << std::endl;
//	std::cout << "sum: " << sum << std::endl;
//	if (distNW == 0)
//		return NW.z;
//	if (distSW == 0)
//		return SW.z;
//	if (distSE == 0)
//		return SW.z;
//	if (distNE == 0)
//		return NE.z;


	//might be bad, consider corners
//		const auto z =
//			NW.z * distNW / sum +
//					SW.z * distSW / sum +
//					SE.z * distSE / sum +
//					NE.z * distNE / sum;
//	const auto z =
//			NW.point.z * (step / (distNW / sum)) +
//					SW.point.z * (step / (distSW / sum)) +
//					SE.point.z * (step / (distSE / sum)) +
//					NE.point.z * (step / (distNE / sum));

}

template <typename DistFn>
libv::vec3f interpolateNeighbours(float x, float y, DistFn&& ratio, Chunk& chunk, libv::xoroshiro128& range) {

	const auto r1 = ratio(range);
	const auto r2 = ratio(range);
	const auto r3 = ratio(range);
	const auto r4 = ratio(range);
	const auto point1 = chunk.points[x][y];
	const auto point2 = chunk.points[x][y + 1];
	const auto point3 = chunk.points[x + 1][y];
	const auto point4 = chunk.points[x + 1][y + 1];
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
//	const auto numVertex = config.resolution+1;
//	auto dist = libv::make_uniform_distribution_inclusive(1, config.treeNumber);
//	auto chunkSize = libv::make_uniform_distribution_exclusive(0, numQuad);
//	auto chunkSize = libv::make_uniform_distribution_exclusive(0, numQuad);
//	auto veggie = libv::make_uniform_distribution_inclusive(0, 3);
	auto ratio = libv::make_uniform_distribution_exclusive(0.f, 1.f);
//	const auto treeCount = dist(range);
	for (const auto& object : config.objects) {
		SurfaceObjectStorage surfaceObjectStorage;
		surfaceObjectStorage.type = object.type;
		surfaceObjectStorage.points.reserve(object.count);
		for (int i = 0; i < object.count; ++i) {
			const auto x = ratio(range);
			const auto y = ratio(range);
//			const auto point = interpolateNeighbours(x, y, ratio, chunk, range);
//			const auto z = 0;
			const auto z = getHeight({x, y}, chunk);
			surfaceObjectStorage.points.emplace_back(libv::vec3f{x + chunk.position.x, y + chunk.position.y, z}, object.size, object.color);
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
	std::cout << "chunk.size: " << chunk.size << std::endl;
	chunk.points.resize(numVertex);


	libv::mt::parallel_for(threads, 0, numVertex, [&](auto yi) {
//				libv::xoroshiro128 rng(seed + i);
//				auto dist = libv::make_uniform_distribution_inclusive(-1.f, 1.f);
		std::vector<SurfacePoint> temp;
		temp.resize(numVertex);

		const auto yf = static_cast<float>(yi);
		const auto size_f = static_cast<float>(numQuad);
		for (int xi = 0; xi < numVertex; ++xi) {
			const auto xf = static_cast<float>(xi);

			const auto noise_value = config.rootNode->evaluate(xf / size_f + chunkPosition.x, yf / size_f + chunkPosition.y);
			const auto point = libv::vec3f{xf / size_f, yf / size_f, noise_value * 0.1f};
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
			temp[xi] = SurfacePoint{point + libv::vec3f{chunkPosition, 0}, color};
//					chunk.points[i][j] = SurfacePoint{libv::vec3f{i_f, j_f, noise_value * 0.1f}, color};
		}
		chunk.points[yi] = std::move(temp);
	});
	return chunk;

}

} // namespace surface
