//// File: utf8.hpp Author: Vader Created on 2017. január 19., 1:07

#pragma once

// ext
//#include <range/v3/detail/satisfy_boost_range.hpp>
#include <range/v3/view/view.hpp>
#include <range/v3/view_facade.hpp>
// std
#include <string_view>


namespace libv {
namespace view {

// -------------------------------------------------------------------------------------------------

namespace detail {

struct view_lines_string_view : ranges::view_facade<view_lines_string_view, ranges::finite> {
	friend ranges::range_access;

private:
	char newline;
	std::string_view range;
	std::string_view::iterator it_begin;
	std::string_view::iterator it_end;

private:
	constexpr inline std::string_view read() const noexcept {
		return std::string_view{it_begin, static_cast<size_t>(std::distance(it_begin, it_end))};
	}
	constexpr inline bool equal(ranges::default_sentinel) const noexcept {
		return it_begin == range.end();
	}
	constexpr inline bool equal(const view_lines_string_view& other) const noexcept {
		return other.it_begin == it_begin && other.it_end == it_end;
	}
	constexpr inline void next() noexcept {
		it_begin = it_end;

		while (true) {
			if (*it_end == newline) { // safe to dereference it_end as sentinel was already checked
				++it_end; // place end after the newline char
				return;
			}

			++it_end;

			if (it_end == range.end())
				return;
		}
	}

public:
	inline view_lines_string_view() noexcept {}
	constexpr inline view_lines_string_view(const std::string_view range, const char newline = '\n') noexcept :
		newline(newline),
		range(range),
		it_begin(range.begin()),
		it_end(range.begin()) {

		next();
	}
};

} // namespace detail

// -------------------------------------------------------------------------------------------------

struct lines_string_view_fn {
	friend ranges::view::view_access;

private:
	template <typename = void>
	static constexpr auto bind(lines_string_view_fn fn, const char newline = '\n') {
		return ranges::make_pipeable([fn, newline](const std::string_view rng) {
			return fn(rng, newline);
		});
	}

public:
	constexpr inline auto operator()(const std::string_view rng, const char newline = '\n') const {
		return detail::view_lines_string_view(rng, newline);
	}
};

/// Includes the new line characters
/// Every line will contain at least the newline character (no empty line will be yielded)
static constexpr ranges::view::view<lines_string_view_fn> lines_string_view{};

// -------------------------------------------------------------------------------------------------

} // namespace view
} // namespace libv

//RANGES_SATISFY_BOOST_RANGE(::libv::view::detail::view_lines_string_view)
