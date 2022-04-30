// Created by dbobula on 4/4/2022.

#pragma once

//libv
#include <libv/math/step.hpp>
#include <utility>
#include <libv/utility/random/xoroshiro128.hpp> // fwdable
#include <libv/utility/random/uniform_distribution.hpp>
#include <libv/container/flat_set.hpp>

//space
#include <surface/surface/config.hpp>

#include <cmath>
#include <optional>
//std
#include <iostream>


namespace surface {
enum class ForrestType {
	forest,  // wet > 0.66
	scrubland, // wet > 0.33
	//grassland/savanna
	desert // wet > 0
};


enum class VeggieClass {
	evergreen,
	tree,
	sapling, //facsemete
	bush, //bokor
//	shrub, //cserje
//	scrub, //bozotos/cserjes
	fern // pafrany
// ice, rock, grass

};

//struct Entry {
//	libv::vec3f position;
//	libv::vec3f normal;
//	float rotation;
//	float height or scale;
//	libv::vec3f hsv_color_shift;
//	int id;
//};

using VeggieId = int32_t;
inline VeggieId globalId = 0;

class Veggie {
public:
	VeggieId id;

	//id
	libv::vec3f pos;
	//vec3 normal vector (up) / kvaternio
	libv::vec3f normal;
	float rotation;
	float scale;
	//clamp during randomization, except hue <- modf
	libv::vec3f hsv_color_shift;

	VeggieType type;

public:
	Veggie(VeggieId id, float rotation, float scale, const libv::vec3f& hsvColorShift, const VeggieType& type);
};


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

} // namespace surface
