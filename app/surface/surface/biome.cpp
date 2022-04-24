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

	std::array<CandidateBiome, 5> candidates;
	candidates.fill(CandidateBiome{nullptr, 0.f, point});

	std::ranges::partial_sort_copy(biomes, candidates,
			std::greater<>{},
			[&](const Biome& biome) { return biome.dominance / (biome.coord - point).length(); },
			[&](const CandidateBiome& biome) { return biome.weight; }
	);

	BiomeMix result;
	int i = 0;
	const auto smallest = candidates[candidates.size() - 1];
	std::vector<WeightedEntry> entries;
	for (const auto& candidate : candidates) {
		if (candidate.biome == nullptr)
			break;

		//TODO: remap so highest remains the old, stretch out to original ratio
		const auto weight = candidate.weight - smallest.weight;
		const auto& entry = WeightedEntry(candidate.biome, candidate.weight);
		entries.emplace_back(entry);
	}
	result.container = WeightedContainer<WeightedEntry>(entries);
	return result;
}


} // namespace surface
