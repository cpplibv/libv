// Project: libv, File: app/surface/surface/biome.cpp

// hpp
#include <surface/surface/biome.hpp>
// std
#include <algorithm>
#include <ranges>
#include <utility>
// libv
#include <libv/algo/erase_if_stable.hpp>
#include <libv/container/flat_set.hpp>
#include <libv/math/constants.hpp>
#include <libv/math/step.hpp>
#include <libv/utility/random/uniform_distribution.hpp>
#include <libv/utility/random/xoroshiro128.hpp>
// pro
#include <surface/log.hpp>
#include <surface/surface/veggie.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

float BiomeMix::normalize() {
	float sum = 0.f;
	for (const auto& entry : entries) {
		sum += entry.weight;
	}

	if (sum == 0.f) {
		log_surface.fatal("Sum == 0.f");
		return 0.f;
	}
//	assert(sum != 0.f);
//	if (sum == 0) {
//		entries[0].weight = 1.f;
//		return;
//	}

	for (auto& entry : entries) {
		entry.weight /= sum;
	}

	return sum;
}


void BiomeMix::applyTakeover() {
	for (auto& entry : entries) {
		const auto takeover = entry.biome->takeover;
//		auto treshold1 = 0.5f;
		if (entry.weight > takeover.soft) {
			auto diff = libv::smoothstep(entry.weight, takeover.soft, takeover.hard) / (1 - entry.weight);
			entry.weight += diff;
//			for (auto& item : entries) {
//
//			}
		}
	}
	auto sum = normalize();

	for (auto& entry : entries | std::views::reverse) {
		const auto handover = entry.biome->handover;
		if (entry.weight < handover.soft) {
			const auto diff = libv::smoothstep(entry.weight, handover.hard, handover.soft);
			const auto sumDiff = entry.weight - entry.weight * diff;
			if (sum - sumDiff > 0.f) {
				entry.weight *= diff;
				sum -= sumDiff;
			}
			else {
				auto little = 0.001f;
				entry.weight = little;
				sum -= entry.weight - little;
			}
		}
	}
	normalize();
}

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
	auto ratio = libv::make_uniform_distribution_exclusive(0.f, 1.f);
	auto number = ratio(rng);
	for (const auto& veggieType : biome.vegetation) {
		if (number < veggieType.probability) {
			return veggieType;
		}
		number -= veggieType.probability;
	}
	return std::nullopt;
}

std::optional<Veggie> BiomeMix::getRandomVeggie(const Biome& biome, libv::xoroshiro128& rng) {
	if (auto veggieType = getRandomVeggieType(biome, rng)) {

		auto normalRDist = libv::make_uniform_distribution_exclusive(0.f, libv::tau);
//		auto normalUDist = libv::make_uniform_distribution_exclusive(0.f, libv::pi / 2.f);
		auto normalUDist = libv::make_uniform_distribution_exclusive(0.f, libv::pi / 2.f / 12.f);
		const auto r = normalRDist(rng);
		const auto u = normalUDist(rng);
		const auto normal = libv::vec3f(
				std::sin(u) * std::cos(r),
				std::sin(u) * std::sin(r),
				std::cos(u)
		);

		auto rotationDist = libv::make_uniform_distribution_exclusive(0.f, libv::tau);
		const auto rotation = rotationDist(rng);

		auto scaleDist = libv::make_uniform_distribution_inclusive(veggieType->scale.min, veggieType->scale.max);
		const auto scale = scaleDist(rng);

		auto hueDist = libv::make_uniform_distribution_inclusive(veggieType->hue.offset - veggieType->hue.radius, veggieType->hue.offset + veggieType->hue.radius);
		const auto hue = std::fmod(hueDist(rng), 360.f) / 360.f;

		auto saturationDist = libv::make_uniform_distribution_inclusive(veggieType->saturation.offset - veggieType->saturation.radius, veggieType->saturation.offset + veggieType->saturation.radius);
		const auto saturation = std::clamp(saturationDist(rng), -1.f, 1.f);

		auto valueDist = libv::make_uniform_distribution_inclusive(veggieType->value.offset - veggieType->value.radius, veggieType->value.offset + veggieType->value.radius);
		const auto value = std::clamp(valueDist(rng), -1.f, 1.f);

		return Veggie(veggieType->modelID, {}, normal, {}, rotation, scale, libv::vec3f{hue, saturation, value});
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
	return BiomeMix(std::move(entries));
}

// -------------------------------------------------------------------------------------------------

} // namespace surface
