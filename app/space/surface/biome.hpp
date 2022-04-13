// Created by dbobula on 4/4/2022.

#pragma once

//libv
#include <libv/math/smoothstep.hpp>
#include <utility>
#include <libv/utility/random/xoroshiro128.hpp> // fwdable
#include <libv/utility/random/uniform_distribution.hpp>
#include <libv/container/flat_set.hpp>

//space
#include <space/surface/config.hpp>

#include <cmath>
//std
#include <iostream>



namespace surface {
enum class ForrestType {
	forest,  // wet > 0.66
	scrubland, // wet > 0.33
	//grassland/savanna
	desert // wet > 0
};


enum class VeggieType {
	evergreen,
	tree,
	sapling, //facsemete
	bush, //bokor
//	shrub, //cserje
//	scrub, //bozotos/cserjes
	fern // pafrany
// ice, rock, grass

};

//class WeightedEntry {
//public:
//	Biome biome;
//	float weight;
//
//public:
//	WeightedEntry(Biome biome, float weight) : biome(std::move(biome)), weight(weight) {};
//};

class BiomeMix {
public:
	struct WeightedEntry {
		const Biome* biome = nullptr;
		float weight;
	};

public:
	std::array<WeightedEntry, 4> entries;

public:
	BiomeMix() {};

	[[nodiscard]] WeightedEntry& operator[](size_t index) {
		return entries[index];
	}

	[[nodiscard]] inline const Biome& primary()  noexcept {
		assert(entries[0].biome != nullptr);
//		assert(entries[1].biome != nullptr);
		std::ranges::sort(entries, std::greater<>{}, &BiomeMix::WeightedEntry::weight);
		std::cout << "result[0].biome->name: " << entries[0].biome->name;
		std::cout << "result[0].weight: " << entries[0].weight << std::endl;
		std::cout << "result[1].biome->name: " << entries[1].biome->name;
		std::cout << "result[1].weight: " << entries[1].weight << std::endl;
		return *entries[0].biome;
	}
//	BiomeMix(std::array<WeightedEntry, 4> candidates_):candidates(candidates_) {};

	//Biome pick();
//	Biome evaluate();
//	Vegi evaluate(libv::xoroshiro128& rng);
};

class BiomePicker {
public:
//	libv::flat_set<Biome> biomes;
//	std::array<CandidateBiome, 4> output;
//	libv::xoroshiro128 rng{123};

public:
	explicit BiomePicker();
	[[nodiscard]] BiomeMix mix(const libv::flat_set<Biome>& biomes, const libv::vec2f point);
	//[[nodiscard]] Biome categorize(const libv::vec2f point);
};

////[0,1]
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
