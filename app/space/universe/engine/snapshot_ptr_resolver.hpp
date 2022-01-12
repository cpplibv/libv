// Created by Vader on 2021.12.20..

#pragma once

// ext
#include <cereal/cereal.hpp>
// libv
#include <libv/utility/entity/entity_ptr_fwd.hpp>
// std
#include <concepts>
#include <type_traits>
// pro
#include <space/universe/fwd.hpp>
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
	libv::entity_ptr<Faction> resolve(FactionID id) const;
	libv::entity_ptr<Planet> resolve(PlanetID id) const;
	libv::entity_ptr<Fleet> resolve(FleetID id) const;
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

template <class T>
inline void CEREAL_SERIALIZE_FUNCTION_NAME(SnapshotPtrResolverArchive&, cereal::BinaryData<T>&) {
	// Visitation is no-op, type serializers will call into the archive
}

// -------------------------------------------------------------------------------------------------

} // namespace space
