// Project: libv, File: app/space/universe/universe.hpp

#pragma once

// fwd
//#include <space/fwd.hpp>
// ext
//#include <boost/container/flat_set.hpp>
// libv
//#include <libv/math/constants.hpp>
#include <libv/math/vec.hpp>
#include <libv/math/vec_serial.hpp>
//#include <libv/math/quat.hpp>
//#include <libv/math/quat_serial.hpp>
//#include <libv/serial/archive/binary_fwd.hpp>
//#include <libv/serial/codec_message_id.hpp>
#include <libv/serial/serial.hpp>
//#include <libv/serial/types/boost_flat_map.hpp>
//#include <libv/serial/types/boost_flat_set.hpp>
#include <libv/serial/types/std_string.hpp>
// std
//#include <map>
//#include <vector>
#include <string>
// pro
#include <space/universe/ids.hpp>
#include <space/universe/engine/chrono.hpp>
//#include <space/universe/colony.hpp>
//#include <space/universe/planet_traits.hpp>

namespace space {

// -------------------------------------------------------------------------------------------------

struct Planet {
	PlanetID id = invalidPlanetID;
	libv::vec3f position;
//	std::string name;
//	uint64_t seed;
	float radius;
//	float probability;
	libv::vec4f color0;
	libv::vec4f color1;
//	float mass;

//	std::optional<Colony> colony;

public:
	Planet() = default; /// For de-serialization only
	Planet(PlanetID id, libv::vec3f position) :
		id(id),
		position(position) {}

public:
	template <class Archive> void serialize(Archive& ar) {
		ar & LIBV_NVP(id);
		ar & LIBV_NVP(position);
//		ar & LIBV_NVP(name);
		ar & LIBV_NVP(radius);
		ar & LIBV_NVP(color0);
		ar & LIBV_NVP(color1);
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
