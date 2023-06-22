// Project: libv.range, File: src/libv/range/view_split_sv.hpp

#pragma once

// ext
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/facade.hpp>
// std
#include <string_view>


namespace libv {
namespace view {

// -------------------------------------------------------------------------------------------------

namespace detail {

struct view_split_sv : ranges::view_facade<view_split_sv, ranges::finite> {
	friend ranges::range_access;

private:
	std::string_view::value_type separator;
	std::string_view::const_pointer range_end;
	std::string_view::const_pointer it_begin;
	std::string_view::const_pointer it_end;

private:
	constexpr inline std::string_view read() const noexcept {
		return std::string_view{it_begin + 1, static_cast<std::size_t>(it_end - it_begin - 1)};
	}
	constexpr inline bool equal(ranges::default_sentinel_t) const noexcept {
		return it_begin == range_end;
	}
	constexpr inline bool equal(const view_split_sv& other) const noexcept {
		return other.it_begin == it_begin && other.it_end == it_end;
	}
	constexpr inline void next() noexcept {
		it_begin = it_end;

		if (it_end != range_end)
			++it_end;

		while (it_end != range_end && *it_end != separator)
			++it_end;
	}

public:
	constexpr inline view_split_sv() noexcept {}
	constexpr inline view_split_sv(const std::string_view range, const std::string_view::value_type separator = '\n') noexcept :
		separator(separator),
		range_end(range.data() + range.size()),
		it_begin(range.data() - 1),
		it_end(range.data()) {

		while (it_end != range.end() && *it_end != separator)
			++it_end;
	}
};

} // namespace detail

// -------------------------------------------------------------------------------------------------

struct split_sv_base_fn {
	static constexpr auto operator()(const std::string_view rng, const std::string_view::value_type newline = '\n') const {
		return detail::view_split_sv(rng, newline);
	}
};

struct split_sv_fn : split_sv_base_fn {
	using split_sv_base_fn::operator();

	static constexpr auto operator()(const std::string_view::value_type newline = '\n') const {
		return ranges::make_view_closure(ranges::bind_back(split_sv_base_fn{}, newline));
	}
};

// -------------------------------------------------------------------------------------------------

/// Excludes the separator characters
/// Every line will contain at least the newline character (no empty line will be yielded)
static constexpr ranges::view::view_closure<split_sv_fn> split_sv{};

// -------------------------------------------------------------------------------------------------

} // namespace view
} // namespace libv

//#include <range/v3/detail/satisfy_boost_range.hpp>
//RANGES_SATISFY_BOOST_RANGE(::libv::view::detail::view_lines_string_view)
