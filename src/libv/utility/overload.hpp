// Project: libv.utility, File: src/libv/utility/overload.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// Creates an overload set based on every parameter's operator()
/// @usage
///     auto x = libv::overload(
///         [](int) { return 1; },
///         [](float) { return 2; }
///     );
template <typename... Args>
inline auto overload(Args&&... args) {
	struct OverloadSet : std::decay_t<Args>... {
		using std::decay_t<Args>::operator()...;
	};

	return OverloadSet{std::forward<Args>(args)...};
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
