// Project: libv.container, File: src/libv/container/flat_set.hpp

#pragma once

// ext
#include <boost/container/flat_set.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T, typename Comp = std::less<>>
using flat_set = boost::container::flat_set<T, Comp>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
