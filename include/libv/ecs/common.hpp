// File: ecs.hpp Author: Vader Created on 2017. március 15., 3:05

#pragma once

// std
#include <bitset>


namespace libv {
namespace ecs {

// -------------------------------------------------------------------------------------------------

using ComponentSystemID = uint32_t;
using FlagID = uint32_t;
using EntityID = uint64_t;
using EntityFlags = std::bitset<32>;
using EntityComponents = std::bitset<64>;

template <size_t... N>
constexpr auto make_bitset() {
	EntityComponents result;
	(result.set(N), ...);
	return result;
}

template <typename... Components>
constexpr auto make_required_bitset() {
	EntityComponents result;
	[[maybe_unused]] auto set_bit = [&result](auto is_optional, auto ID) {
		if (!is_optional)
			result.set(ID);
	};
	(set_bit(Components::is_optional_v, Components::ID), ...);
	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace ecs
} // namespace libv