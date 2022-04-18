// Created by dbobula on 4/4/2022.

#include <space/surface/biome.hpp>

#include <utility>
#include <algorithm>


namespace surface {
// -------------------------------------------------------------------------------------------------

//template <typename T>
//[[nodiscard]] const T& random(const libv::xoroshiro128& rng) noexcept {
//	auto ratio = libv::make_uniform_distribution_inclusive(0.f, 1.f);
//	auto number = ratio(rng);
//	for (const auto& entry : entries) {
//		if (number < entry.weight)
//			return *entry.biome;
//		number -= entry.weight;
//	}
//	throw std::runtime_error("BiomeMix.random() has reached end unexpectedly");
//}

// -------------------------------------------------------------------------------------------------

BiomePicker::BiomePicker() {}

BiomeMix BiomePicker::mix(const libv::flat_set<Biome>& biomes, const libv::vec2f point) {
	struct CandidateBiome {
		const Biome* biome;
		float distance_sq;
		libv::vec2f point; // Hacky

		void operator=(const Biome& source) {
			biome = &source;
			distance_sq = (source.coord - point).length_sq();
		}
	};

	std::array<CandidateBiome, 5> candidates;
	candidates.fill(CandidateBiome{nullptr, std::numeric_limits<float>::infinity(), point});

	std::ranges::partial_sort_copy(biomes, candidates,
			std::less<>{},
			[&](const Biome& biome) { return (biome.coord - point).length_sq(); },
			[&](const CandidateBiome& biome) { return biome.distance_sq; }
	);

	BiomeMix result;
	float sum = 0.f;
	size_t i = 0;
//	bool smallest = true;
//blending trick doesnt work when there is only 2 biomes
	float smallestWeight = std::numeric_limits<float>::infinity();

	for (const auto& candidate : candidates) {
		if (candidate.biome == nullptr) {
			continue;
		}
		const auto dist = std::sqrt(candidate.distance_sq);
		const auto weight = candidate.biome->dominance / dist;
		if (weight < smallestWeight)
			smallestWeight = weight;
	}

	for (int j = candidates.size() - 1; j >= 0; --j) {
		const auto& candidate = candidates[j];
		if (candidate.biome == nullptr) {
//			std::cout << "i: " << i << std::endl;
			continue;
		}

		//
		const auto dist = std::sqrt(candidate.distance_sq);
//		const auto weight = dist;
//		const auto weight = std::max(0.01f,-candidate.biome->dominance * dist + 1);
//		const auto weight = candidate.biome->dominance / (dist + candidate.biome->dominance);
//		const auto weight = (1 - dist) * candidate.biome->dominance;
		const auto weight = candidate.biome->dominance / dist;
		if (weight == smallestWeight) {
			continue;
		}
		result[i].biome = candidate.biome;
		//TODO: remap so highest remains the old, stretch out to original ratio
		result[i].weight = weight - smallestWeight;
//		result[i].weight = 1.f - dist;
//		result[i].weight = candidate.biome->dominance;
		sum += result[i].weight;

		i++;
	}
//	for (const auto& candidate : candidates) {
//		if (candidate.biome == nullptr) {
////			std::cout << "i: " << i << std::endl;
//			break;
//		}
//		const auto dist = std::sqrt(candidate.distance_sq);
//		result[i].biome = candidate.biome;
//		result[i].weight = candidate.biome->dominance / dist;
////		result[i].weight = 1.f - dist;
////		result[i].weight = candidate.biome->dominance;
//		sum += result[i].weight;
//
//		i++;
//	}

	///normalization
	for (auto& entry : result.entries) {
		if (entry.biome == nullptr)
			continue;

		entry.weight /= sum;
//		entry.weight = 1 - entry.weight;
//		if (entry.biome.cutOff.x > ratio)
//			entry = nullptr;
//		if (entry.biome.cutOff.y < ratio) {
//			result.emplace(entry.biome, ratio);
//			return result;
//		}
	}

//	float sum2 = 0.f;
//	for (const auto& entry : result.entries) {
//		sum2 += entry.weight;
//	}
////	std::cout << "sum2: " << sum2 << std::endl;
//	assert(sum2 > 0.99f);
//	assert(sum2 < 1.01f);

	std::ranges::sort(result.entries, std::greater<>{}, &BiomeMix::WeightedEntry::weight);


	return result;
}

} // namespace surface
