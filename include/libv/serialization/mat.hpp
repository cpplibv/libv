// File:   mat.hpp Author: Vader Created on 2017. február 26., 15:57

#pragma once

// ext
#include <boost/serialization/array.hpp>
// libv
#include <libv/math/mat.hpp>
#include <libv/serialization/nvp.hpp>

namespace boost {
namespace serialization {

// -------------------------------------------------------------------------------------------------

template <typename Archive, size_t N, typename T>
inline void serialize(Archive& ar, ::libv::mat_t<N, T>& var, const unsigned int) {
	ar & LIBV_NVP_NAMED("mat", boost::serialization::make_array(libv::value_ptr(var), N * N));
}

// -------------------------------------------------------------------------------------------------

} // namespace serialization
} // namespace boost