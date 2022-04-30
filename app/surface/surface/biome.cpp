// Created by dbobula on 4/4/2022.

#include <surface/surface/biome.hpp>

#include <utility>
#include <algorithm>

#include <libv/algo/erase_if_stable.hpp>


namespace surface {
// -------------------------------------------------------------------------------------------------

Veggie::Veggie(VeggieId id, float rotation, float scale, const libv::vec3f& hsvColorShift, const VeggieType& type) :
		id(id), rotation(rotation), scale(scale), hsv_color_shift(hsvColorShift), type(type) {}

// -------------------------------------------------------------------------------------------------

BiomeMix::BiomeMix() {};

void BiomeMix::normalize() {
	float sum = 0.f;
	for (const auto& entry : entries) {
		sum += entry.weight;
	}

	assert(sum != 0.f);

	for (auto& entry : entries) {
		entry.weight /= sum;
	}
}


void BiomeMix::blendForVeggies() {
	//cut-off
	//TODO: This breaks the blending
	bool winner = false;
	WeightedEntry winnerEntry;
	for (auto& entry : entries) {
//		if (entry.biome == nullptr)
//			continue;
		assert(entry.biome != nullptr);

		if (entry.weight > entry.biome->cutOff.y) {
			winnerEntry = entry;
			winner = true;
			break;
		}
	}
	if (winner) {
		entries.clear();
		winnerEntry.weight = 1.0f;
		entries.emplace_back(winnerEntry);
	} else {
		size_t i = 0;
		libv::erase_if_stable(entries, [&i, this](const auto& x) {
			assert(x.biome != nullptr);
			const auto result = x.weight < x.biome->cutOff.x && i + 2 < entries.size();
			if (result)
				++i;
			return result;
		});
		normalize();
	}
}

//void BiomeMix::blendForTiles() {
//	const auto& smallest = entries[entries.size() - 1];
//	container.erase(smallest);
//}

const Biome& BiomeMix::primary() noexcept {

	assert(entries[0].biome != nullptr);
	return *entries[0].biome;
}

const Biome& BiomeMix::random(libv::xoroshiro128& rng) {
	//TODO: Extract weighted selection algorithm with projection
	auto ratio = libv::make_uniform_distribution_exclusive(0.f, 1.f);
	auto number = ratio(rng);
	for (const auto& entry : entries) {
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
	for (const auto& entry : entries) {
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

std::optional<Veggie> BiomeMix::getRandomVeggie(const Biome& biome, libv::xoroshiro128& rng) {
	if (auto veggieType = getRandomVeggieType(biome, rng)) {

		auto rotationScale = libv::make_uniform_distribution_inclusive(0.f, 360.f);
		auto rotation = std::fmod(rotationScale(rng), 360.f);

		auto scaleRange = libv::make_uniform_distribution_inclusive(veggieType->scale.min, veggieType->scale.max);
		auto scale = scaleRange(rng);

		auto hueShift = libv::make_uniform_distribution_inclusive(veggieType->hue.offset - veggieType->hue.radius, veggieType->hue.offset + veggieType->hue.radius);
		auto hue = std::fmod(hueShift(rng), 360.f);

		auto saturationShift = libv::make_uniform_distribution_inclusive(veggieType->saturation.offset - veggieType->saturation.radius, veggieType->saturation.offset + veggieType->saturation.radius);
		auto saturation = std::clamp(saturationShift(rng), 0.f, 1.f);

		auto valueShift = libv::make_uniform_distribution_inclusive(veggieType->value.offset - veggieType->value.radius, veggieType->value.offset + veggieType->value.radius);
		auto value = std::clamp(valueShift(rng), 0.f, 1.f);


		return Veggie(globalId, rotation, scale, libv::vec3f{hue, saturation, value}, *veggieType);
	}

	return std::nullopt;
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

	const auto smallest = candidates[candidates.size() - 1];
	std::vector<WeightedEntry> entries;
	for (const auto& candidate : candidates) {
		if (candidate.biome == nullptr)
			break;

		//TODO: remap so highest remains the old, stretch out to original ratio
		const auto weight = candidate.weight - smallest.weight;
		const auto& entry = WeightedEntry(candidate.biome, weight);
		entries.emplace_back(entry);
	}
	return BiomeMix(entries);
}

} // namespace surface
