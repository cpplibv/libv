// File: mat.hpp Author: Vader Created on 2017. február 26., 15:57

#pragma once

// hpp
#include <libv/math/vec.hpp>
// libv
#include <libv/serialization/serialization.hpp>


namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE {

// -------------------------------------------------------------------------------------------------

template <typename Archive, size_t R, size_t C, typename T>
inline void serialize(Archive& ar, ::libv::mat_t<R, C, T>& var, const unsigned int) {
	for (size_t i = 0; i < R * C; i++)
		ar(var.data()[i]);
}

// -------------------------------------------------------------------------------------------------

} // namespace LIBV_SERIALIZATION_EXTENSION_NAMESPACE
