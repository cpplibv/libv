// Project: libv, File: app/surface/surface/biome.hpp

#pragma once

// libv
#include <libv/container/flat_set_fwd.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/random/xoroshiro128_fwd.hpp>
// std
#include <optional>
// pro
#include <surface/surface/config.hpp>
#include <surface/surface/fwd.hpp>
//#include <surface/surface/biome.hpp>


namespace surface {

// -------------------------------------------------------------------------------------------------

struct WeightedEntry {
	const Biome* biome = nullptr;
	float weight = 0.f;

	WeightedEntry() {}

	WeightedEntry(const Biome* biome, float weight) : biome(biome), weight(weight) {}

	bool operator==(const WeightedEntry& other) const {
		return other.biome == biome;
	}
};

class BiomeMix {
private:
//	libv::xoroshiro128 rng{123};
private:
//	std::array<WeightedEntry, 4> entries;
//	WeightedContainer<WeightedEntry> container;
	std::vector<WeightedEntry> entries;

public:
	explicit BiomeMix(std::vector<WeightedEntry>&& entries) : entries(std::move(entries)) {
		normalize();
		applyTakeover();
	}

public:
	[[nodiscard]] const Biome& primary() noexcept;
	[[nodiscard]] const Biome& random(libv::xoroshiro128& rng);
	[[nodiscard]] libv::vec4f blendedColor(float fertility) noexcept;
	[[nodiscard]] std::optional<Veggie> getRandomVeggie(const Biome& biome, libv::xoroshiro128& rng);

private:
	float normalize();
	void applyTakeover();
	[[nodiscard]] std::optional<VeggieType> getRandomVeggieType(const Biome& biome, libv::xoroshiro128& rng);

};

class BiomePicker {
private:
	struct CandidateBiome {
		const Biome* biome;
		float weight;
//		float distance_sq;
		libv::vec2f point; // Hacky

	public:
		CandidateBiome() {}
		CandidateBiome(const Biome* biome, float weight, libv::vec2f point) :
				biome(biome), weight(weight), point(point) {}

		void operator=(const Biome& source) {
			biome = &source;
			weight = source.dominance / (source.coord - point).length();
		}
	};

public:
//	std::array<CandidateBiome, 4> output;

public:
	explicit BiomePicker();
//	Biome* emptyBiome();

	[[nodiscard]] BiomeMix mix(const libv::flat_set<Biome>& biomes, const libv::vec2f point);
//	[[nodiscard]] BiomeMix veggieMix(const libv::flat_set<Biome>& biomes, const libv::vec2f point);
	//[[nodiscard]] Biome categorize(const libv::vec2f point);
};

//[0,1]
//inline float calculateFertility(float tempStep, float tempBiomeZoneMin, float wetStep, float wetForrestMin,
//		float height, float temp, float wet,
//		float fertilityOffset, float heightSensitivity) {
//	const auto tempRelative = (temp - tempBiomeZoneMin) / tempStep - height * heightSensitivity;
//	const auto wetRelative = (wet - wetForrestMin) / wetStep;
//	return libv::smoothstep((tempRelative + wetRelative) / 2.f, 0.f, 1.f);
////	return std::max(0.f, libv::smoothstep((tempRelative + wetRelative) / 2.f, 0.f,1.f) - (1.f - fertilityOffset));
////	return std::max(0.f, (tempRelative + wetRelative) / 2.f  - barrenness);
////	return std::min((tempRelative + wetRelative) / 2.f , fertility);
////	return tempRelative * wetRelative ;
////	return tempRelative * wetRelative ;
//}

// -------------------------------------------------------------------------------------------------

} // namespace surface
