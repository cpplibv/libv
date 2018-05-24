// File:   source_location.hpp Author: Vader Created on 2018.05.24., 19:43

#pragma once

// std
#include <experimental/source_location>


namespace libv {

// -------------------------------------------------------------------------------------------------

using source_location = std::experimental::source_location;

/// Proxy type with implicit constructor to be called from a functions where call site where
/// a default argument for source_location is not possible, and at least one argument present
/// that type can be fixed and bundle with the source_location information.
/// @usage \code
///	template <typename... Args>
/// void error(const with_source_location<const std::string_view> fmt, const Args&... args) {
///		fmt.value;
///		fmt.location;
///		args...;
///	}
/// \endcode
template <typename T>
struct with_source_location {
	T value;
	source_location location;

	// Implicit constructor to be used for conversion
	template <typename CT>
	with_source_location(CT&& value, source_location location = source_location::current()) :
		value(std::forward<CT>(value)),
		location(location) { }
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
