// Project: libv.meta, File: src/libv/meta/similar.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <type_traits>


// TODO P5: make Similar variadic

namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename S, typename T>
using similar = std::is_same<std::decay_t<S>, std::decay_t<T>>;

template <typename S, typename T>
static constexpr bool similar_v = std::is_same_v<std::decay_t<S>, std::decay_t<T>>;

template <typename S, typename T>
using Similar = std::enable_if_t<similar<S, T>::value>;

template <typename S, typename T>
using NotSimilar = std::enable_if_t<!similar<S, T>::value>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
