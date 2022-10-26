// Project: libv.container, File: src/libv/container/flat_set_fwd.hpp

#pragma once

// ext
#include <boost/container/container_fwd.hpp>
// std
#include <functional> // For std::less fwd only


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T, typename Comp = std::less<>>
using flat_set = boost::container::flat_set<T, Comp>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
