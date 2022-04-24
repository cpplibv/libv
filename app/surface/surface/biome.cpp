// Created by dbobula on 4/4/2022.

#include <surface/surface/biome.hpp>

#include <utility>
#include <algorithm>


namespace surface {
// -------------------------------------------------------------------------------------------------

BiomeMix::BiomeMix() {};

void BiomeMix::blendForVeggies() {
	//cut-off
	//TODO: This breaks the blending
	bool winner = false;
	for (auto& entry : container.entries) {
//		if (entry.biome == nullptr)
//			continue;
		assert(entry.biome != nullptr);

		if (entry.weight > entry.biome->cutOff.y) {
			container = WeightedContainer<WeightedEntry>();
			container.insert(entry);
			winner = true;
			break;
		}
	}
//
	if (!winner) {
		std::vector<WeightedEntry> toBeErased;
		for (auto& entry : container.entries) {
//		if (entry.biome == nullptr)
//			continue;
			assert(entry.biome != nullptr);
			if (container.entries.size() == 1 + toBeErased.size())
				break;

			if (entry.weight < entry.biome->cutOff.x) {
				toBeErased.emplace_back(entry);
//			container.erase(entry);

			}
		}
		container.eraseAll(toBeErased);
	}
}

//void BiomeMix::blendForTiles() {
//	const auto& smallest = container.entries[container.entries.size() - 1];
//	container.erase(smallest);
//}

const Biome& BiomeMix::primary() noexcept {

	assert(container.entries[0].biome != nullptr);
	return *container.entries[0].biome;
}

const Biome& BiomeMix::random(libv::xoroshiro128& rng) {
	//TODO: Extract weighted selection algorithm with projection
	auto ratio = libv::make_uniform_distribution_exclusive(0.f, 1.f);
	auto number = ratio(rng);
	for (const auto& entry : container.entries) {
		assert(entry.biome != nullptr);
		if (number <= entry.weight)
			return *entry.biome;
		number -= entry.weight;
	}
	return primary();

	//TODO: reached error, fix
	//make_uniform_distribution_inclusive -> exclusive solved it?
//		throw std::runtime_error("BiomeMix.random() has reached end unexpectedly");
}

libv::vec4f BiomeMix::blendedColor(float fertility) noexcept {
	libv::vec4f result;
	for (const auto& entry : container.entries) {
		assert(entry.biome != nullptr);
		result += entry.biome->colorGrad.sample(fertility) * entry.weight;
	}
	return result;
}


std::optional<VeggieType> BiomeMix::getRandomVeggieType(const Biome& biome, libv::xoroshiro128& rng) {
//		float sum = 0.f;
//		for (const auto & veggieType : biome.vegetation) {
//			sum += veggieType.probability;
//		}
	auto ratio = libv::make_uniform_distribution_exclusive(0.f, 1.f);
	auto number = ratio(rng);
//		VeggieType result;
	for (const auto& veggieType : biome.vegetation) {
		if (number < veggieType.probability) {
//				result = veggieType;
			return veggieType;
		}
		number -= veggieType.probability;
	}
	return std::nullopt;
//		throw std::runtime_error("BiomeMix.getRandomVeggieType() has reached end unexpectedly");
}


// -------------------------------------------------------------------------------------------------

BiomePicker::BiomePicker() {}

//Biome* BiomePicker::emptyBiome() {
//	Biome result;
//	result.name = "empty";
//	result.coord = {0, 0};
////	result.cutOff = table["cutOff"];
//	result.dominance = 0.f;
//	libv::gradientf<libv::vec4f> colorGrad;
//	colorGrad.add(0.f, libv::vec4f{1, 1, 1, 0});
//	colorGrad.add(1.f, libv::vec4f{0, 0, 0, 0});
//
//	result.colorGrad = colorGrad;
////	result.vegetation = convertVeggieTypes(table["vegetation"]);
//
//	return result;
//}

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
