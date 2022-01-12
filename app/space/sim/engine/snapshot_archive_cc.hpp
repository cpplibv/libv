// Created by Vader on 2021.12.20..

#pragma once

// std
#include <concepts>


namespace space {

// -------------------------------------------------------------------------------------------------

template <typename Archive>
concept ccSnapshotArchive = requires(const Archive& car) {
//	// TODO P5: Basic Input / Output Archive stuff
	{ car.isLocal() } -> std::same_as<bool>;
	{ car.isShared() } -> std::same_as<bool>;
};

// -------------------------------------------------------------------------------------------------

} // namespace space
