// File: overload.hpp - Created on 2018.01.02. 12:10 - Author: Vader

#pragma once

// std
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// Creates an overload set based on every parameter's operator()
/// @usage
///     auto x = overload(
///         [](int) { return 1; },
///         [](float) { return 2; }
///     );
template <typename... Args>
auto overload(Args&&... args) {
	struct OverloadSet : std::decay_t<Args>... {
		using std::decay_t<Args>::operator()...;
	};

	return OverloadSet{std::forward<Args>(args)...};
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
