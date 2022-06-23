// Project: libv, File: app/space/sim/engine/snapshot_archive.hpp

#pragma once

// ext
#include <vide/archives/proxy_archive.hpp>
// std
#include <utility>
// pro
#include <space/sim/engine/serial_id.hpp>
#include <space/sim/engine/snapshot_type.hpp>



namespace space {

// -------------------------------------------------------------------------------------------------

class SimulationContext;

/// SnapshotArchive is an archive wrapper that includes SnapshotType and the SimulationContext:
/// SnapshotType - determines the usage of the snapshot
/// SimulationContext - can be used for object allocation and id <-> pointer resolving
///
/// \Usage
/// 		SnapshotArchive<libv::archive::BasicBinaryInput> iar(false, message);
template <typename Archive>
class SnapshotArchive : public vide::ProxyArchive<SnapshotArchive<Archive>, Archive> {
	using Base = vide::ProxyArchive<SnapshotArchive<Archive>, Archive>;

public:
	SimulationContext& simulationContext;

private:
	SnapshotType snapshotType;
//	SnapshotType snapshotTypeSource;
//	SnapshotType snapshotTypeTarget;

public:
	explicit inline SnapshotArchive(Archive& archive, SimulationContext& simulationContext, SnapshotType snapshotType) :
		Base(archive),
		simulationContext(simulationContext),
		snapshotType(snapshotType) {
//		Base(snapshotTypeSource);
	}

//public:
//	template <typename T>
//	inline SnapshotArchive& operator()(T&& var) {
//		return Base::operator()(std::forward<T>(var));
//	}
//
//	template <typename T>
//	inline SnapshotArchive& operator()(T&& var, SnapshotType varType) {
//		if (Base::is_input && snapshotTypeSource == varType) {
//			T dummy; // Read in and discard
//			return Base::operator()(dummy);
//		} else
//			return *this;
//	}

public:
	[[nodiscard]] constexpr inline bool isLocal() const noexcept {
		return snapshotType == SnapshotType::local;
	}
	[[nodiscard]] constexpr inline bool isShared() const noexcept {
		return snapshotType == SnapshotType::shared;
	}

public:
	using Base::process_as;

	template <typename As, typename T>
	inline void process_as(As& as, libv::primary_entity_ptr<T>& var) {
		simulationContext.memory.create_to(var);
		as(*var);
	}

	template <typename As, typename T>
	inline void process_as(As& as, const libv::primary_entity_ptr<T>& var) {
		as(*var);
	}
////		using id_type = SerialID<T>::id_type;
//		using underlying_type = SerialID<T>::underlying_type;
//
//		if constexpr(As::is_output) {
//			if (var.ptr == nullptr)
//				as(static_cast<underlying_type>(-1));
//			else
//				as(static_cast<underlying_type>(var.ptr->id));
//
//		} else {
//			underlying_type id;
//			as(id);
//			var.ptr.adopt(libv::int_to_ptr<T>(id));
//		}
//	}
//
//	template <typename As, typename T>
//	inline void process_as(As& as, const SerialID<T>& var) {
////		using id_type = SerialID<T>::id_type;
//		using underlying_type = SerialID<T>::underlying_type;
//
//		if constexpr(As::is_output) {
//			if (var.ptr == nullptr)
//				as(static_cast<underlying_type>(-1));
//			else
//				as(static_cast<underlying_type>(var.ptr->id));
//
//		} else {
//			underlying_type id;
//			as(id);
//			var.ptr.adopt(libv::int_to_ptr<T>(id));
//		}
//	}
};

// -------------------------------------------------------------------------------------------------

} // namespace space
