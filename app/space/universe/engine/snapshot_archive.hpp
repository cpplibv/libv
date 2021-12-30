// Created by Vader on 2021.12.20..

#pragma once

// std
#include <concepts>


namespace space {

// -------------------------------------------------------------------------------------------------

template <typename Archive>
concept cxSnapshotArchive = requires(const Archive& car) {
//	// TODO P5: Basic Input / Output Archive stuff
	{ car.isLocal() } -> std::same_as<bool>;
	{ car.isShared() } -> std::same_as<bool>;
};

// -------------------------------------------------------------------------------------------------

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
