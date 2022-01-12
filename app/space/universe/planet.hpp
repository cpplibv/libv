// Project: libv, File: app/space/universe/universe.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/utility/entity/entity_ptr.hpp>
// std
#include <string>
// pro
#include <space/universe/engine/chrono.hpp>
#include <space/universe/engine/entity.hpp>
#include <space/universe/engine/screen_pickable_type.hpp>
#include <space/universe/fwd.hpp>
#include <space/universe/ids.hpp>
//#include <space/universe/colony.hpp>
//#include <space/universe/planet_traits.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

class Planet : public Entity<Planet, PlanetID, invalidPlanetID> {
public:
	static constexpr inline ScreenPickableType pickingType{0.75f, 80.f};

public:
	libv::vec3f position;
	std::string name;
//	uint64_t seed;
	float radius = 1.f;
//	float probability;
	libv::vec4f color0;
	libv::vec4f color1;
//	float mass;

	libv::entity_ptr<Faction> faction;

//	std::optional<Colony> colony;

public:
	Planet(); /// For de-serialization only
	Planet(PlanetID id, libv::vec3f position);
	~Planet();

public:
	template <typename Archive> void serialize(Archive& ar);

public:
	void kill();
	void update(sim_duration dt);
};

// -------------------------------------------------------------------------------------------------

} // namespace space
