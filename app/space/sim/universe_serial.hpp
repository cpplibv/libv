// Project: libv, File: app/space/sim/universe_serial.hpp

#pragma once

// hpp
#include <space/sim/universe.hpp>
// libv
#include <libv/serial/serial.hpp>
#include <libv/utility/random/xoroshiro128_serial.hpp>
// pro
#include <space/sim/engine/snapshot_archive_cc.hpp>
#include <space/sim/engine/snapshot_ptr_resolver.hpp>
#include <space/sim/galaxy_serial.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

template <typename Archive> void Universe::save(Archive& ar) const {
	static_assert(ccSnapshotArchive<Archive>);

	ar & LIBV_NVP(universe_rng);
	ar & LIBV_NVP(galaxy);
}

template <typename Archive> void Universe::load(Archive& ar) {
	static_assert(ccSnapshotArchive<Archive>);

	ar & LIBV_NVP(universe_rng);
	ar & LIBV_NVP(galaxy);

	SnapshotPtrResolverArchive resolver{*this, ar.isLocal()};

	resolver & LIBV_NVP(universe_rng);
	resolver & LIBV_NVP(galaxy);
}

// -------------------------------------------------------------------------------------------------

} // namespace space
