// Project: libv.range, File: src/libv/range/view_construct.hpp

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
	static constexpr inline auto operator()(Rng&& rng) const {
		return ranges::view::transform(libv::construct<T, uint32_t>);;
	}

	static constexpr inline auto operator()() const {
		return [](auto&& rng) {
			using Rng = decltype(rng);
			return ranges::view::transform(std::forward<Rng>(rng), libv::construct<T, uint32_t>);
		};
	}
};

template <typename T>
static constexpr ranges::view::view_closure<construct_fn<T>> construct{};

// -------------------------------------------------------------------------------------------------

} // namespace view
} // namespace libv
