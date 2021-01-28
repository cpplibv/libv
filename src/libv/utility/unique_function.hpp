// Project: libv.utility, File: src/libv/utility/unique_function.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <function2/function2.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Signature>
using unique_function = fu2::unique_function<Signature>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
