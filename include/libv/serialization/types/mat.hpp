// File: mat.hpp Author: Vader Created on 2017. február 26., 15:57

#pragma once

// hpp
#include <libv/math/vec.hpp>
// libv
#include <libv/serialization/serialization.hpp>


namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE {

// -------------------------------------------------------------------------------------------------

template <typename Archive, size_t N, typename T>
inline void serialize(Archive& ar, ::libv::mat_t<N, T>& var, const unsigned int) {
	for (size_t i = 0; i < N * N; ++i)
		ar(::libv::value_ptr(var)[i]);
}

// -------------------------------------------------------------------------------------------------

} // namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE

