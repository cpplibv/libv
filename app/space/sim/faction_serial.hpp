// Project: libv, File: src/space/universe/force.hpp

#pragma once

// hpp
#include <space/sim/faction.hpp>
// libv
#include <libv/math/vec_serial.hpp>
#include <libv/serial/serial.hpp>
#include <libv/serial/types/std_string.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

template <typename Archive> void Faction::serialize(Archive& ar) {
	ar & LIBV_NVP(id);
	ar & LIBV_NVP(name);
	ar & LIBV_NVP(colorPrimary);
	ar & LIBV_NVP(colorSecondary);
}

// -------------------------------------------------------------------------------------------------

} // namespace space
