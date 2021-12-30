// Project: libv, File: app/space/universe/universe.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/math/vec_serial.hpp>
#include <libv/serial/serial.hpp>
#include <libv/serial/types/std_memory.hpp>
#include <libv/serial/types/std_string.hpp>
// std
//#include <optional>
#include <memory>
#include <string>
//#include <map>
//#include <vector>
// pro
#include <space/universe/engine/chrono.hpp>
#include <space/universe/engine/screen_pickable_type.hpp>
#include <space/universe/faction.hpp>
#include <space/universe/ids.hpp>
//#include <space/universe/colony.hpp>
//#include <space/universe/planet_traits.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

struct Planet {
public:
	static constexpr inline ScreenPickableType pickingType{0.75f, 80.f};

public:
	PlanetID id = invalidPlanetID;
	libv::vec3f position;
	std::string name;
//	uint64_t seed;
	float radius = 1.f;
//	float probability;
	libv::vec4f color0;
	libv::vec4f color1;
//	float mass;

//	FactionID ownerFaction;
//	std::optional<FactionID> ownerFaction;
	std::shared_ptr<Faction> faction;

//	std::optional<Colony> colony;

public:
	Planet() = default; /// For de-serialization only
	Planet(PlanetID id, libv::vec3f position) :
			id(id),
			position(position) {}

public:
	template <typename Archive> void serialize(Archive& ar) {
		ar & LIBV_NVP(id);
		ar & LIBV_NVP(position);
		ar & LIBV_NVP(name);
		ar & LIBV_NVP(radius);
		ar & LIBV_NVP(color0);
		ar & LIBV_NVP(color1);
		ar & LIBV_NVP(faction);
	}

public:
	void update(sim_duration dt) {
		(void) dt;
//		if (colony)
//			colony->update(dt);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace space
