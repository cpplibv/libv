// Project: libv.utility, File: src/libv/utility/projection.hpp

#pragma once

// std
#include <functional>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Func>
[[nodiscard]] constexpr inline auto proj_indirect(Func&& func) noexcept {
	return [f = std::forward<Func>(func)](auto&& var) mutable -> decltype(auto) {
		return std::invoke(f, *var);
	};
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
