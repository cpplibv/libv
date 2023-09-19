// Project: libv.meta, File: src/libv/meta/assignable_to.hpp

#include <concepts>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Lhs, typename Rhs>
concept assignable_to = std::assignable_from<Rhs, Lhs>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
