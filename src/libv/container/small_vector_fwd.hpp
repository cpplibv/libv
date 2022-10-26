// Project: libv.container, File: src/libv/container/small_vector_fwd.hpp

#pragma once

// ext
#include <boost/container/container_fwd.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T, int N>
using small_vector = boost::container::small_vector<T, N>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
