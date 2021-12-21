// Created by Vader on 2021.12.20..

#pragma once

// ext
#include <cereal/cereal.hpp>
// std
#include <concepts>
#include <type_traits>
// pro
#include <space/fwd.hpp>
#include <space/universe/ids.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

class SnapshotPtrResolverArchive : public cereal::OutputArchive<SnapshotPtrResolverArchive, cereal::IgnoreNVP> {
private:
	const Universe& universe;
	bool isLocal_;

public:
	explicit inline SnapshotPtrResolverArchive(const Universe& universe, bool isLocal) :
			cereal::OutputArchive<SnapshotPtrResolverArchive, cereal::IgnoreNVP>(this),
			universe(universe),
			isLocal_(isLocal) {}

public:
	[[nodiscard]] constexpr inline bool isLocal() const noexcept {
		return isLocal_;
	}
	[[nodiscard]] constexpr inline bool isShared() const noexcept {
		return !isLocal_;
	}

public:
//	Faction* resolve(FactionID id) const;
	Planet* resolve(PlanetID id) const;
	Fleet* resolve(FleetID id) const;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
inline void CEREAL_SERIALIZE_FUNCTION_NAME(SnapshotPtrResolverArchive& ar, cereal::NameValuePair<T>& var) {
	ar(var.value);
}

template <typename T>
	requires (std::is_arithmetic_v<T>)
inline void CEREAL_SERIALIZE_FUNCTION_NAME(SnapshotPtrResolverArchive&, T&) {
	// Visitation is no-op, type serializers will call into the archive
}

template <typename T>
inline void CEREAL_SERIALIZE_FUNCTION_NAME(SnapshotPtrResolverArchive&, cereal::SizeTag<T>&) {
	// Visitation is no-op, type serializers will call into the archive

}

/// Loading binary data from portable binary
template <class T>
inline void CEREAL_SERIALIZE_FUNCTION_NAME(SnapshotPtrResolverArchive&, cereal::BinaryData<T>&) {
	// Visitation is no-op, type serializers will call into the archive
}

// -------------------------------------------------------------------------------------------------

/// SerialID can be used during serialization to save the pointers ID instead of its value
/// During deserialization an output archive (yes, output and not input, due to input would modify everything)
/// can be used with this: T* resolve(decltype(T.id) id) signature to reverse look up IDs to pointers
///
/// \Usage
///		template <class Archive> void serialize(Archive& ar) {
///			ar & LIBV_NVP(position);
///			ar & LIBV_NVP_NAMED("target", SerialID{&target});
///		}
///
template <typename T>
class SerialID {
	friend cereal::access;

private:
	using id_type = decltype(std::declval<T>().id);
	using underlying_type = std::underlying_type_t<id_type>;

public:
	T** ptr;

private:
	template <class Archive>
	inline underlying_type save_minimal(const Archive&) const {
		if (*ptr == nullptr)
			return -1;
		else
			return static_cast<underlying_type>((*ptr)->id);
	}

	template <class Archive>
	inline void load_minimal(const Archive&, const underlying_type& id) {
		*ptr = reinterpret_cast<T*>(id);
	}

	template <class Archive>
	inline underlying_type save_minimal(Archive& ar) const
			requires requires { { ar.resolve(std::declval<id_type>()) } -> std::same_as<T*>; } {

		// The id was already saved in place of the pointer: grab it, resolve it, place it back
		const auto id = id_type{*reinterpret_cast<underlying_type*>(ptr)};

		// Const cast
		*ptr = const_cast<Archive&>(ar).resolve(id);

		return 0; // Return value will be discarded
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace space
