//// File: utf8.hpp Author: Vader Created on 2017. január 19., 1:07

#pragma once

// ext
#include <range/v3/view/all.hpp>
#include <range/v3/view/view.hpp>
#include <range/v3/view_facade.hpp>
#include <utf8cpp/utf8.h>
// std
#include <string>


namespace libv {
namespace view {

// -------------------------------------------------------------------------------------------------

namespace detail {

// Note: range v3 currently freaks out if it sees a constructor in view_facade with only one
//		const string& as argument, the current workaround is separation for now.
class aux_uft8_codepoints_view : public ranges::view_facade<aux_uft8_codepoints_view> {
	friend ranges::range_access;

	std::string::const_iterator iter_;
	std::string::const_iterator end_;

	uint32_t read() const {
		return utf8::peek_next(iter_, end_);
	}
	bool equal(ranges::default_sentinel) const {
		return iter_ == end_;
	}
	void next() {
		utf8::next(iter_, end_);
	}

public:
	inline aux_uft8_codepoints_view() :
		iter_(),
		end_(iter_) { }
	inline aux_uft8_codepoints_view(std::string::const_iterator iter_, std::string::const_iterator end_) :
		iter_(iter_),
		end_(end_) { }
};

struct uft8_codepoints_view : aux_uft8_codepoints_view {
	uft8_codepoints_view() = default;
	using aux_uft8_codepoints_view::aux_uft8_codepoints_view;
	inline explicit uft8_codepoints_view(const std::string& s) :
		aux_uft8_codepoints_view(s.begin(), s.end()) { }
};

} // namespace detail

// -------------------------------------------------------------------------------------------------

struct uft8_codepoints_fn {

	template <typename Rng>
	auto operator()(Rng&& rng) const {
		return detail::uft8_codepoints_view(std::forward<Rng>(rng));
	}
	auto operator()() const {
		return [](auto&& rng) {
			using Rng = decltype(rng);
			return detail::uft8_codepoints_view(std::forward<Rng>(rng));
		};
	}
};

RANGES_INLINE_VARIABLE(ranges::view::view<uft8_codepoints_fn>, uft8_codepoints)

// -------------------------------------------------------------------------------------------------

} // namespace view
} // namespace libv
