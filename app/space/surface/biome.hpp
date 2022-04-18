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


using VeggieId = int32_t;

class Veggie {
public:
	//id
	VeggieId id;
	libv::vec3f pos;
	//vec3 normal vector (up) / kvaternio
	libv::vec3f normal;
	float rotation;
	float height;
	libv::vec3f hsvDiff;

	VeggieType type;
	//size
	//colorDiff
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
private:
//	libv::xoroshiro128 rng{123};
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

	[[nodiscard]] inline const Biome& primary() noexcept {
		assert(entries[0].biome != nullptr);
		return *entries[0].biome;
	}

	[[nodiscard]] inline const Biome& random(libv::xoroshiro128& rng) {
		//TODO: Extract weighted selection algorithm with projection
		auto ratio = libv::make_uniform_distribution_exclusive(0.f, 1.f);
		auto number = ratio(rng);
		for (const auto& entry : entries) {
			if (number <= entry.weight)
				return *entry.biome;
			number -= entry.weight;
		}
		return primary();

		//TODO: reached error, fix
		//make_uniform_distribution_inclusive -> exclusive solved it?
//		throw std::runtime_error("BiomeMix.random() has reached end unexpectedly");
	}

	[[nodiscard]] inline const libv::vec4f blendedColor(float fertility) noexcept {
		libv::vec4f result;
		for (const auto& entry : entries) {
			if (entry.biome == nullptr) {
				continue;
			}
			result += entry.biome->colorGrad.sample(fertility) * entry.weight;
		}
		return result;
	}


//	normalize(probalities)
//	veggieType=getWeigthedRandom
//	veggie=makeRandomVeggie(veggieType, biome?, fertility?)
//	return veggie
	[[nodiscard]] inline std::optional<VeggieType> getRandomVeggieType(const Biome& biome, libv::xoroshiro128& rng) {
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

	[[nodiscard]] inline std::optional<Veggie> getRandomVeggie(const Biome& biome, libv::xoroshiro128& rng);
//		if(const auto type = getRandomVeggieType(biome, rng)){
//			Veggie result;
//			result.type = type.value();
//			result.id = ;
//			result.height = ;
//
//			result.hsvDiff = ;
//			result.rotation = ;
//		}
//
//
//	}
//		auto type = getRandomVeggieType(biome, rng);
//		return Veggie()
//
//
//	}
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
