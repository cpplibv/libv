//// File: utf8.hpp Author: Vader Created on 2017. január 19., 1:07

#pragma once

// ext
#include <range/v3/view.hpp>
#include <range/v3/view/transform.hpp>
//libv
#include <libv/utility/construct.hpp>


namespace libv {
namespace view {

// -------------------------------------------------------------------------------------------------

template <typename T>
struct construct_fn {
	template <typename Rng>
	constexpr inline auto operator()(Rng&& rng) const {
		return ranges::view::transform(libv::construct<T, uint32_t>);;
	}
	constexpr inline auto operator()() const {
		return [](auto&& rng) {
			using Rng = decltype(rng);
			return ranges::view::transform(std::forward<Rng>(rng), libv::construct<T, uint32_t>);
		};
	}
};

template <typename T>
constexpr inline ranges::view::view<construct_fn<T>> construct{};

// -------------------------------------------------------------------------------------------------

} // namespace view
} // namespace libv