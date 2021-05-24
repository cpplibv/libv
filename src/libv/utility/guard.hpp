// Project: libv.utility, File: src/libv/utility/guard.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <utility>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// @usage \code
/// [[nodiscard]] inline auto push_guard() noexcept {
/// 	push();
/// 	return libv::guard([this] { pop(); });
/// } \endcode
///
/// @usage \code
/// auto foo() {
/// 	push();
/// 	auto pop_guard = libv::guard([&] { pop(); });
/// 	// ... Lot of code that might exit in many ways
/// } \endcode
template <typename F>
class guard {
private:
	F action;

public:
	guard() = delete;
	guard(const guard&) = delete;
	guard& operator=(const guard&) & = delete;
	guard(guard&&) = delete;
	guard& operator=(guard&&) & = delete;

	template <typename CF>
	inline explicit guard(CF&& func) : action(std::forward<CF>(func)) {}

	inline ~guard() {
		action();
	}
};

template <typename F>
guard(F) -> guard<F>;

// -------------------------------------------------------------------------------------------------

template <typename F>
using scope_guard = guard<F>;

// -------------------------------------------------------------------------------------------------

} // namespace libv
