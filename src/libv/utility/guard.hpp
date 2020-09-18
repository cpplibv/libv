// Project: libv.utility, File: src/libv/utility/guard.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <utility>


namespace libv {

// -------------------------------------------------------------------------------------------------

// TODO P3: rename Guard file and class to scope_guard

/// @usage \code
/// [[nodiscard]] inline auto push_guard() noexcept {
/// 	push();
/// 	return libv::Guard([this] { pop(); });
/// } \endcode
template <typename F>
class Guard {
	F action;
public:
	Guard() = delete;
	Guard(const Guard&) = delete;
	Guard(Guard&&) = delete;
	Guard& operator=(const Guard&) = delete;
	Guard& operator=(Guard&&) = delete;

	template <typename CF>
	inline explicit Guard(CF&& func) : action(std::forward<CF>(func)) {}

	inline ~Guard() {
		action();
	}
};

template <typename F>
Guard(F) -> Guard<F>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
