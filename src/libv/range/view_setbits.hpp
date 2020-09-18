//// File: utf8.hpp Author: Vader Created on 2017. január 19., 1:07

#pragma once

// ext
#include <range/v3/view/facade.hpp>
// std
#include <bit>
#include <type_traits>


namespace libv {
namespace view {

// -------------------------------------------------------------------------------------------------

namespace detail {

template <typename Integral>
struct setbits_view : ranges::view_facade<setbits_view<Integral>, ranges::finite> {
	friend ranges::range_access;
	using Index = int;
	using Rep = std::make_unsigned_t<Integral>;

private:
	Rep integral = 0;
	Index it = 0; // Points to the current (non zero) bit if valid or to a zero bit if its the end of range
	static constexpr Index it_end = sizeof(Rep) * 8;

private:
	constexpr inline Index read() const noexcept {
		return it;
	}
	constexpr inline bool equal(ranges::default_sentinel_t) const noexcept {
		return (integral >> it) == 0;
	}
	constexpr inline bool equal(const setbits_view& other) const noexcept {
		return other.it == it;
	}
	constexpr inline void next() noexcept {
		it += std::countr_zero(static_cast<Rep>(integral >> (it + 1))) + 1;
	}
//	constexpr inline void prev() noexcept {
//		it -= std::countl_zero(static_cast<Rep>(integral >> it)) + 1;
//	}

public:
	constexpr inline setbits_view() noexcept {}
	constexpr inline setbits_view(const Integral integral) noexcept : integral(static_cast<Rep>(integral)) {
		it = std::countr_zero(this->integral);
	}

	constexpr inline auto size() noexcept {
		return std::popcount(integral);
	}
};

} // namespace detail

// -------------------------------------------------------------------------------------------------

struct setbits_fn {
	template <typename Integral>
	constexpr inline auto operator()(const Integral integral) const noexcept {
		return detail::setbits_view<Integral>(integral);
	}
};

static constexpr setbits_fn setbits{};

// -------------------------------------------------------------------------------------------------

} // namespace view
} // namespace libv

#include <range/v3/detail/satisfy_boost_range.hpp>
RANGES_SATISFY_BOOST_RANGE(::libv::view::detail::setbits_view)
