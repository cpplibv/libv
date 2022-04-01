// Project: libv.utility, File: src/libv/utility/ordering.hpp

#pragma once

// std
#include <compare>


namespace libv {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] constexpr inline int ordering_as_int(std::strong_ordering cmp) noexcept {
    return (cmp < 0) ? -1 : ((cmp == 0) ? 0 : 1);
}

[[nodiscard]] constexpr inline int ordering_as_int(std::weak_ordering cmp) noexcept {
    return (cmp < 0) ? -1 : ((cmp == 0) ? 0 : 1);
}

[[nodiscard]] constexpr inline int ordering_as_int(std::partial_ordering cmp) noexcept {
    return (cmp < 0) ? -1 : ((cmp == 0) ? 0 : 1);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
