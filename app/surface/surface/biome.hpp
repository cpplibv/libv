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
//template<class T>
//float optionalToString(T* obj)
//{
//	constexpr bool has_toString = requires(const T& t) {
//		t.weight;
//	};
//
//	if constexpr (has_toString)
//		return obj->toString();
//	else
//		return 1.0f;
//}


template <typename T>
//template <typename T>
requires requires(const T entry){ entry.weight; }
//concept ccSnapshotArchive = requires(const T entry) {
//	{ entry.weight } -> std::same_as<float>;
//	{ car.isShared() } -> std::same_as<bool>;
//};
class WeightedContainer {
public:
	std::vector<T> entries;

private:

	void normalize() {
		float sum = 0.f;
		for (const auto& entry : entries) {
			sum += entry.weight;
		}

		assert(sum != 0.f);

		for (auto& entry : entries) {
			entry.weight /= sum;
		}
	}

public:
	WeightedContainer() {
		entries.reserve(5);
	}

	explicit WeightedContainer(std::vector<T> entries_) {
		entries = std::vector<T>(entries_.begin(), entries_.end());
		normalize();
	}

//	explicit WeightedContainer(const std::vector<T>& entries) : entries(entries) {}

	void insert(const T& entry) {
		entries.emplace_back(entry);
		normalize();
	}

	void erase(const T& entry) {
		const auto it = std::find(entries.begin(), entries.end(), entry);
		if (it == entries.end())
			assert(false);

		entries.erase(it);
		normalize();
	}

	void eraseAll(const std::vector<T>& entries_) {
		for (const auto& entry : entries_) {
			const auto it = std::find(entries.begin(), entries.end(), entry);
			if (it == entries.end())
				assert(false);

			entries.erase(it);
		}
		normalize();
	}
//	void addAll()
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
			if (number <= entry.weight || entry.weight >= entry.biome->cutOff.y)
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
