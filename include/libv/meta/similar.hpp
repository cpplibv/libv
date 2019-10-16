// File:   similar.hpp Author: Vader Created on 2018. május 31., 0:31

#pragma once

// std
#include <type_traits>


// TODO P5: make Similar variadic

namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename S, typename T>
using similar = std::is_same<std::decay_t<S>, std::decay_t<T>>;

template <typename S, typename T>
static constexpr bool similar_v = similar<S, T>::value;

template <typename S, typename T>
using Similar = std::enable_if_t<similar<S, T>::value>;

template <typename S, typename T>
using NotSimilar = std::enable_if_t<!similar<S, T>::value>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
