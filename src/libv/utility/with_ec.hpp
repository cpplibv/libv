// Project: libv.utility, File: src/libv/utility/with_ec.hpp

#pragma once

// std
#include <system_error>
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename F, typename... Args>
auto with_ec(F&& func, Args&&... args) {
	using Result = std::invoke_result_t<F, Args..., std::error_code&>;
	std::error_code ec;
	if constexpr (std::is_same_v<Result, void>) {
		std::forward<F>(func)(std::forward<Args>(args)..., ec);
		return ec;
	} else {
		Result result = std::forward<F>(func)(std::forward<Args>(args)..., ec);
		return std::pair<Result, std::error_code>{std::forward<Result>(result), ec};
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
