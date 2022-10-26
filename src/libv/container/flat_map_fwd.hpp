// Project: libv.container, File: src/libv/container/flat_map_fwd.hpp

#pragma once

// ext
#include <boost/container/container_fwd.hpp>
// std
#include <functional> // For std::less fwd only


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename K, typename V, typename Comp = std::less<>>
using flat_map = boost::container::flat_map<K, V, Comp>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
