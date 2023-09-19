// Project: libv.meta, File: src/libv/meta/similar.hpp

#pragma once

#include <concepts>
#include <type_traits>


// TODO P5: make Similar variadic

namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename S, typename T>
using similar = std::is_same<std::decay_t<S>, std::decay_t<T>>;

template <typename S, typename T>
static constexpr bool similar_v = std::is_same_v<std::decay_t<S>, std::decay_t<T>>;

// -------------------------------------------------------------------------------------------------

template <typename S, typename T>
concept similar_as = std::same_as<std::remove_cvref_t<S>, std::remove_cvref_t<T>>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
