// Created by dbobula on 4/4/2022.

#include <surface/surface/biome.hpp>

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
		float weight;
//		float distance_sq;
		libv::vec2f point; // Hacky

		void operator=(const Biome& source) {
			biome = &source;
			weight = source.dominance / (source.coord - point).length();
		}
	};

	std::array<CandidateBiome, 5> candidates;
	candidates.fill(CandidateBiome{nullptr, 0.f, point});

	std::ranges::partial_sort_copy(biomes, candidates,
			std::less<>{},
			[&](const Biome& biome) { return biome.dominance / (biome.coord - point).length(); },
			[&](const CandidateBiome& biome) { return biome.weight; }
	);

	BiomeMix result;
	float sum = 0.f;
	int i = 0;
//	const auto last = candidates[candidates.size()-1];
	const auto smallestWeight = candidates[candidates.size()-1].weight;
	for (const auto& candidate : candidates) {
		if (candidate.biome == nullptr)
			break;

		result[i].biome = candidate.biome;
		//TODO: remap so highest remains the old, stretch out to original ratio
		result[i].weight = candidate.weight - smallestWeight;
//		result[i].weight = 1.f - dist;
//		result[i].weight = candidate.biome->dominance;
		sum += result[i].weight;
		i++;
	}

//		const auto weight = dist;
//		const auto weight = std::max(0.01f,-candidate.biome->dominance * dist + 1);
//		const auto weight = candidate.biome->dominance / (dist + candidate.biome->dominance);
//		const auto weight = (1 - dist) * candidate.biome->dominance;

	///normalization
	for (auto& entry : result.entries) {
		if (entry.biome == nullptr)
			continue;

		entry.weight /= sum;
	}
	std::ranges::sort(result.entries, std::greater<>{}, &BiomeMix::WeightedEntry::weight);


	return result;
}

} // namespace surface
