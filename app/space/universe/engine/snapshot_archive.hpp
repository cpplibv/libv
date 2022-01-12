// Created by Vader on 2021.12.20..

#pragma once

// std
#include <utility>


namespace space {

// -------------------------------------------------------------------------------------------------

/// SnapshotArchive is an archive wrapper that includes isLocal/isShared bool property to determine the usage of the snapshot
///
///	Local Snapshot: Includes everything from the simulation. Meant for quick save / load
///	Shared Snapshot: Excludes some local perspective from the simulation (Like selection). Meant to be shared on the network or in a save file
///
/// \Usage
/// 		SnapshotArchive<libv::archive::BasicBinaryInput> iar(false, message);
template <template <typename CRTP> typename Archive>
class SnapshotArchive : public Archive<SnapshotArchive<Archive>> {
	bool isLocal_;

public:
	template <typename... Args>
	explicit inline SnapshotArchive(bool isLocal, Args&&... args) :
		Archive<SnapshotArchive<Archive>>(std::forward<Args>(args)...),
		isLocal_(isLocal) {}

public:
	[[nodiscard]] constexpr inline bool isLocal() const noexcept {
		return isLocal_;
	}
	[[nodiscard]] constexpr inline bool isShared() const noexcept {
		return !isLocal_;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace space
