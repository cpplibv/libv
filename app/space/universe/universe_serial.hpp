// Project: libv, File: app/space/universe/universe.hpp

#pragma once

// hpp
#include <space/universe/universe.hpp>
// libv
#include <libv/serial/serial.hpp>
#include <libv/utility/random/xoroshiro128_serial.hpp>
// pro
#include <space/universe/engine/snapshot_archive_cc.hpp>
#include <space/universe/engine/snapshot_ptr_resolver.hpp>
#include <space/universe/galaxy_serial.hpp>


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
