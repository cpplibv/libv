//// File: utf8.hpp Author: Vader Created on 2017. január 19., 1:07

#pragma once

// ext
#include <range/v3/detail/satisfy_boost_range.hpp>
#include <range/v3/view_facade.hpp>


namespace libv {
namespace view {

// -------------------------------------------------------------------------------------------------

namespace detail {

template <typename Integral>
struct setbits_view : ranges::view_facade<setbits_view<Integral>, ranges::finite> {
	friend ranges::range_access;
	using Index = Integral;

private:
	Integral integral = 0;
	Index it = 0; // Points to the current (non zero) bit if valid or to a zero bit if its the end of range
	static constexpr Index it_max = sizeof(Integral) * 8 - 1;

private:
	constexpr inline Index read() const {
		return it;
	}
	constexpr inline bool equal(ranges::default_sentinel) const {
		return it > it_max || integral < (1 << it);
	}
	constexpr inline void next() {
		// C++20 bit operations, shift with count right zeros, etc...
		//	size = popcount, next = countr_zero + 1
		it++;
		while ((integral & (1 << it)) == 0 && it <= it_max)
			it++;
	}
	constexpr inline bool equal(const setbits_view& other) const {
		return other.it == it;
	}
//	void prev() {
//		--value_;
//	}

public:
	constexpr inline setbits_view() {}
	constexpr inline setbits_view(const Integral integral) : integral(integral) {
		if (integral != 0)
			while ((integral & (1 << it)) == 0)
				it++;
	}
};

} // namespace detail

// -------------------------------------------------------------------------------------------------

struct setbits_fn {
	template <typename Integral>
	constexpr inline auto operator()(const Integral integral) const {
		return detail::setbits_view<Integral>(integral);
	}
};

constexpr inline setbits_fn setbits{};

// -------------------------------------------------------------------------------------------------

} // namespace view
} // namespace libv

RANGES_SATISFY_BOOST_RANGE(::libv::view::detail::setbits_view)
