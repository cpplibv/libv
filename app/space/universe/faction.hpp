// Project: libv, File: src/space/universe/force.hpp

#pragma once

// libv
#include <libv/color/space.hpp>
#include <libv/math/vec.hpp>
#include <libv/serial/serial.hpp>
#include <libv/serial/types/std_string.hpp>
#include <libv/utility/random/uniform_distribution.hpp>
#include <libv/utility/random/xoroshiro128.hpp>
// std
#include <string>
// pro
#include <space/universe/ids.hpp>
#include <space/universe/engine/chrono.hpp>
//#include <space/log.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

class Faction {
private:
public: // <<<
	FactionID id = invalidFactionID;
	std::string name;
	libv::vec4f colorPrimary;
	libv::vec4f colorSecondary;
//	Controller* controller = passive, ai, player;

public:
	template <class Archive> void serialize(Archive& ar) {
		ar & LIBV_NVP(id);
		ar & LIBV_NVP(name);
		ar & LIBV_NVP(colorPrimary);
		ar & LIBV_NVP(colorSecondary);
	}

public:
	Faction() = default; /// For de-serialization only
	Faction(FactionID id, std::string name) :
			id(id),
			name(std::move(name)) {

		if (+id < 13) {
			const auto idf = static_cast<float>(+id) - 1.f;
			colorPrimary = libv::color::hsv_to_rgb({idf / 12.f, 0.75f, 1.f, 1.f});
			colorSecondary = libv::color::hsv_to_rgb({idf / 12.f + 0.5f / 12.f, 0.6f, 1.f, 1.f});

		} else {
			auto rng = libv::xoroshiro128(+id + 500000);
			auto dist = libv::make_uniform_distribution_inclusive(0.f, 1.f);
			libv::vec4f color0hsva;
			color0hsva.x = dist(rng);
			color0hsva.y = dist(rng) * 0.6f + 0.2f;
			color0hsva.z = dist(rng) * 0.4f + 0.6f;
			color0hsva.w = 1.f;
			libv::vec4f color1hsva;
			color1hsva.x = dist(rng);
			color1hsva.y = dist(rng) * 0.6f + 0.2f;
			color1hsva.z = dist(rng) * 0.4f + 0.6f;
			color1hsva.w = 1.f;
			colorPrimary = libv::color::hsv_to_rgb(color0hsva);
			colorSecondary = libv::color::hsv_to_rgb(color1hsva);
		}
	}

	Faction(FactionID id, std::string name, libv::vec4f colorPrimary, libv::vec4f colorSecondary) :
			id(id), name(std::move(name)), colorPrimary(colorPrimary), colorSecondary(colorSecondary) {}

public:
	void update(sim_duration delta_time) {
		(void) delta_time;
	}

public:
	[[nodiscard]] bool canAttack(const Faction& other) const {
		// <<< Use relation matrix
//		log_space.error("Can attack {} -> {}? {}", +id, +other.id, id != other.id);
		return id != other.id;
	}
};

//enum class Relation {
//	ally,
//	friendly,
//	neutral,
//	hostile,
//	atWar,
//
//	unknown,
//};
//
//class FactionRelationMatrix {
////	std::map<std::pair<Faction, Faction>, Relation> relations;
////	std::vector<>
//};

// -------------------------------------------------------------------------------------------------

} // namespace space
