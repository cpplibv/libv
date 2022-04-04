// Created by dbobula on 4/4/2022.

#include <space/surface/biome.hpp>

#include <iostream>
#include <utility>
#include <algorithm>


namespace surface {
// -------------------------------------------------------------------------------------------------



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

	std::array<CandidateBiome, 4> candidates;
	candidates.fill(CandidateBiome{nullptr, std::numeric_limits<float>::infinity(), point});

	std::ranges::partial_sort_copy(biomes, candidates,
			std::less<>{},
			[&](const Biome& biome) { return (biome.coord - point).length_sq(); },
			[&](const CandidateBiome& biome) { return biome.distance_sq; }
	);

	BiomeMix result;
	float sum = 0.f;
	size_t i = 0;
	for (const auto& candidate : candidates) {
		if (candidate.biome == nullptr)
			break;
		const auto dist = std::sqrt(candidate.distance_sq);
		result[i].biome = candidate.biome;
		result[i].weight = 1/dist * candidate.biome->dominance;
		sum += result[i].weight;
	}

	for (auto& entry : result.entries) {
		if (entry.biome == nullptr)
			break;

		entry.weight /= sum; //TODO: invert
//		entry.weight = 1 - entry.weight; //TODO: normalize again
//		if (entry.biome.cutOff.x > ratio)
//			entry = nullptr;
//		if (entry.biome.cutOff.y < ratio) {
//			result.emplace(entry.biome, ratio);
//			return result;
//		}
	}
	return result;
}

} // namespace surface
