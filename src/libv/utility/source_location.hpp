// Project: libv.utility, File: src/libv/utility/source_location.hpp

#pragma once

// std
#include <source_location>
#include <utility>


namespace libv {

// -------------------------------------------------------------------------------------------------

using source_location = std::source_location;

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
	explicit(false) with_source_location(CT&& value, source_location location = source_location::current()) :
		value(std::forward<CT>(value)),
		location(location) { }
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
