// Project: libv.utility, File: src/libv/utility/unique_function.hpp

#pragma once

// ext
#include <function2/function2.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Signature>
using unique_function = fu2::unique_function<Signature>;
// std::move_only_function could be another consideration

// -------------------------------------------------------------------------------------------------

} // namespace libv
