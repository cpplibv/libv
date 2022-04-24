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


public:
//	std::array<WeightedEntry, 4> entries;
	WeightedContainer<WeightedEntry> container;

public:
	BiomeMix();
	void blendForVeggies();
//	void blendForTiles();
//	void normalize();
//	void replaceWithNullEntry(WeightedEntry& entry);
//	void replaceOthersWithNullEntries(WeightedEntry& entry);
//	[[nodiscard]] WeightedEntry& operator[](size_t index) {
//		return container.entries[index];
//	}

	[[nodiscard]] const Biome& primary() noexcept;
	[[nodiscard]] const Biome& random(libv::xoroshiro128& rng);
	[[nodiscard]] libv::vec4f blendedColor(float fertility) noexcept;

	[[nodiscard]] std::optional<VeggieType> getRandomVeggieType(const Biome& biome, libv::xoroshiro128& rng);
	[[nodiscard]] std::optional<Veggie> getRandomVeggie(const Biome& biome, libv::xoroshiro128& rng);

};

class BiomePicker {
private:
	struct CandidateBiome {
		const Biome* biome;
		float weight;
//		float distance_sq;
		libv::vec2f point; // Hacky
		CandidateBiome(){}

		CandidateBiome(const Biome* biome, float weight, libv::vec2f point) :
			biome(biome), weight(weight), point(point) {}

		void operator=(const Biome& source) {
			biome = &source;
			weight = source.dominance / (source.coord - point).length();
		}
	};

public:
//	libv::flat_set<Biome> biomes;
//	std::array<CandidateBiome, 4> output;
//	libv::xoroshiro128 rng{123};

public:
	explicit BiomePicker();
	Biome* emptyBiome();

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
