// Project: libv.range, File: src/libv/range/view_lines_string_view.hpp

#pragma once

// ext
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/range/concepts.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/facade.hpp>
// std
#include <string_view>


namespace libv {
namespace view {

// -------------------------------------------------------------------------------------------------

namespace detail {

struct view_lines_string_view : ranges::view_facade<view_lines_string_view, ranges::finite> {
	friend ranges::range_access;

private:
	std::string_view::value_type newline;
	std::string_view range;
	std::string_view::iterator it_begin;
	std::string_view::iterator it_end;

private:
	constexpr inline std::string_view read() const noexcept {
		return std::string_view{it_begin, static_cast<size_t>(std::distance(it_begin, it_end))};
	}
	constexpr inline bool equal(ranges::default_sentinel_t) const noexcept {
		return it_begin == range.end();
	}
	constexpr inline bool equal(const view_lines_string_view& other) const noexcept {
		return other.it_begin == it_begin && other.it_end == it_end;
	}
	constexpr inline void next() noexcept {
		it_begin = it_end;

		while (it_end != range.end() && *it_end != newline)
			++it_end;

		if (it_end != range.end())
			++it_end;
	}

public:
	constexpr inline view_lines_string_view() noexcept {}
	constexpr inline view_lines_string_view(const std::string_view range, const std::string_view::value_type newline = '\n') noexcept :
		newline(newline),
		range(range),
		it_begin(range.begin()),
		it_end(range.begin()) {

		next();
	}
};

} // namespace detail

// -------------------------------------------------------------------------------------------------

struct lines_string_view_base_fn {
	constexpr auto operator()(const std::string_view rng, const std::string_view::value_type newline = '\n') const {
		return detail::view_lines_string_view(rng, newline);
	}
};

struct lines_string_view_fn : lines_string_view_base_fn {
	using lines_string_view_base_fn::operator();

	constexpr auto operator()(const std::string_view::value_type newline = '\n') const {
		return ranges::make_view_closure(ranges::bind_back(lines_string_view_base_fn{}, newline));
	}
};

/// Includes the new line characters
/// Every line will contain at least the newline character (no empty line will be yielded)
static constexpr ranges::view::view_closure<lines_string_view_fn> lines_string_view{};

// -------------------------------------------------------------------------------------------------

} // namespace view
} // namespace libv

//#include <range/v3/detail/satisfy_boost_range.hpp>
//RANGES_SATISFY_BOOST_RANGE(::libv::view::detail::view_lines_string_view)
