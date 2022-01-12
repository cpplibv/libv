// Created by Vader on 2021.12.20..

#pragma once

// libv
#include <libv/utility/entity/entity_ptr.hpp>
#include <libv/utility/int_to_ptr.hpp>
// std
//#include <concepts>
#include <type_traits>
// pro
#include <space/universe/engine/snapshot_ptr_resolver.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

/// SerialID can be used during serialization to save the pointers ID instead of its value
/// During deserialization an output archive (yes, output and not input, due to input would modify everything)
/// can be used with this: libv::entity_ptr<T> resolve(decltype(T.id) id) signature to reverse look up IDs to pointers
///
/// \Usage
///		libv::entity_ptr<T> target;
///		template <typename Archive> void serialize(Archive& ar) {
///			ar & LIBV_NVP(position);
///			ar & LIBV_NVP_NAMED("target", SerialID{target});
///		}
///
///		template <typename Archive> void top_level_load(Archive& ar) {
///			ar & LIBV_NVP(universe);
///			SnapshotPtrResolverArchive resolver{*this, ar.isLocal()};
///			resolver & LIBV_NVP(universe);
///		}
template <typename T>
class SerialID {
	friend cereal::access;

private:
	using id_type = decltype(std::declval<T&>().id);
	using underlying_type = std::underlying_type_t<id_type>;

public:
	libv::entity_ptr<T>& ptr;

public:
	explicit inline SerialID(libv::entity_ptr<T>& ptr) noexcept : ptr(ptr) {}

private:
	template <typename Archive>
	inline underlying_type save_minimal(const Archive&) const {
		if (ptr == nullptr)
			return -1;
		else
			return static_cast<underlying_type>(ptr->id);
	}

	template <typename Archive>
	inline void load_minimal(const Archive&, const underlying_type& id) {
		ptr.adopt(libv::int_to_ptr<T>(id));
	}

//	template <typename Archive>
//			requires requires (Archive& ar, id_type id){ { ar.resolve(id) } -> std::same_as<T*>; }
//	inline underlying_type save_minimal(const Archive& ar) const {
	inline underlying_type save_minimal(const SnapshotPtrResolverArchive& ar) const {
		// The id was already saved in place of the pointer: grab it, resolve it, place it back
		const auto id = id_type{libv::ptr_to_int<underlying_type>(ptr.abandon())};

		ptr = ar.resolve(id); // NOTE: SerialID is allowed to edit the ptr during "save" as the resolver uses save inside a load, so the object is not const

		return 0; // Return value will be discarded
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace space
