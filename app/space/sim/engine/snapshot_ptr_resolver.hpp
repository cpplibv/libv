// Project: libv, File: app/space/sim/engine/snapshot_ptr_resolver.hpp

#pragma once

// ext
#include <vide/vide.hpp>
#include <vide/concept.hpp>
// libv
#include <libv/utility/entity/entity_ptr_fwd.hpp>
// pro
#include <space/sim/engine/serial_id.hpp>
#include <space/sim/engine/snapshot_type.hpp>
#include <space/sim/fwd.hpp>
#include <space/sim/ids.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

class SnapshotPtrResolverArchive : public vide::OutputArchive<SnapshotPtrResolverArchive, vide::IgnoreNVP> {
private:
//	Universe& universe;
	const Universe& universe;
	SnapshotType snapshotType;

public:
//	explicit inline SnapshotPtrResolverArchive(bool isLocal, const Universe& universe) :
//	explicit inline SnapshotPtrResolverArchive(SimulationContext& simulationContext, SnapshotType snapshotType) :
	explicit inline SnapshotPtrResolverArchive(const Universe& universe, SnapshotType snapshotType) :
//			simulationContext(simulationContext),
			universe(universe),
			snapshotType(snapshotType) {}

public:
	[[nodiscard]] constexpr inline bool isLocal() const noexcept {
		return snapshotType == SnapshotType::local;
	}
	[[nodiscard]] constexpr inline bool isShared() const noexcept {
		return snapshotType == SnapshotType::shared;
	}

public:
	libv::entity_ptr<Faction> resolve(FactionID id) const;
	libv::entity_ptr<Planet> resolve(PlanetID id) const;
	libv::entity_ptr<Fleet> resolve(FleetID id) const;

public:
	using OutputArchive::process_as;

	template <typename As, typename T>
	inline void process_as(As& as, const vide::NameValuePair<T>& var) {
		as(var.value);
	}

	template <typename As, typename T>
	inline void process_as(As&, const vide::SizeTag<T>&) {
		// Visitation is no-op, type serializers will call into the archive
	}

	template <typename As, typename T>
	inline void process_as(As&, const vide::BinaryData<T>&) {
		// Visitation is no-op, type serializers will call into the archive
	}

	template <typename As, vide::arithmetic T>
	inline void process_as(As&, const T&) {
		// Visitation is no-op, type serializers will call into the archive
	}

	template <typename As, typename T>
	inline void process_as(As&, const SerialID<T>& var) {
		using id_type = SerialID<T>::id_type;
		using underlying_type = SerialID<T>::underlying_type;

		// The id was already saved in place of the pointer: grab it, resolve it, place it back
		const auto id = id_type{libv::ptr_to_int<underlying_type>(var.ptr.abandon())};

		// NOTE: SerialID is allowed to edit the ptr during "save" as the resolver uses save inside a load, so the object is not const
		var.ptr = resolve(id);
	}

	template <typename As, typename T>
	inline void process_as(As& as, const libv::primary_entity_ptr<T>& var) {
		as(*var);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace space
