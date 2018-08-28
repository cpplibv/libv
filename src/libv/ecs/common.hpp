// Project: libv.ecs, File: src/libv/ecs/common.hpp

#pragma once

// std
#include <bitset>


namespace libv {
namespace ecs {

// -------------------------------------------------------------------------------------------------

constexpr inline std::size_t MAX_COMPONENT_COUNT = 64;

using ComponentSystemID = uint8_t;
using FlagID = uint32_t;
using EntityID = uint64_t;
using EntityFlags = std::bitset<32>; // TODO P4: unacceptable padding in std::bitset sizeof(std::bitset<32>) == 8
using EntityComponents = std::bitset<MAX_COMPONENT_COUNT>;

template <std::size_t... N>
constexpr inline auto make_bitset() noexcept {
	EntityComponents result;
	(result.set(N), ...);
	return result;
}

template <typename... Components>
constexpr inline auto make_required_bitset() noexcept {
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
