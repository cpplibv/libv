// Project: libv.serial, File: src/libv/serial/limit.hpp

#pragma once

// std
//#include <ranges>


namespace libv {
namespace serial {

// -------------------------------------------------------------------------------------------------

/// Forward compatible placeholder for bound limits
template <typename Range, int ElementLimit>
//	requires std::ranges::range<Range>
using limit = Range;

// -------------------------------------------------------------------------------------------------

} // namespace serial
} // namespace libv
