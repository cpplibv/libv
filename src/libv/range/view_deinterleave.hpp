// Project: libv.range, File: src/libv/range/view_deinterleave.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <meta/meta.hpp>
#include <range/v3/functional/bind_back.hpp>
#include <range/v3/range/concepts.hpp>
#include <range/v3/view/adaptor.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/stride.hpp>
#include <range/v3/view/subrange.hpp>


namespace libv {
namespace view {

// -------------------------------------------------------------------------------------------------

namespace detail {

template <typename Rng>
struct deinterleave_view : ranges::view_adaptor<deinterleave_view<Rng>, Rng> {
private:
	friend ranges::range_access;
	static_assert(ranges::random_access_range<Rng>, "");

	class sentinel_adaptor;

	template<bool Const>
	struct adaptor : ranges::adaptor_base {
	private:
		friend class sentinel_adaptor;
		friend class adaptor<!Const>;
		using CRng = ::meta::const_if_c<Const, Rng>;

		ranges::range_difference_type_t<CRng> n_;
		ranges::sentinel_t<CRng> end_;
		ranges::iterator_t<CRng> stride_end_;

	public:
		adaptor() = default;

		constexpr adaptor(::meta::const_if_c<Const, deinterleave_view>& cv) :
			n_((RANGES_EXPECT(0 < cv.n_), cv.n_)),
			end_(ranges::end(cv.base())),
			stride_end_(ranges::next(ranges::begin(cv.base()), n_, end_)) {}

		constexpr adaptor(const adaptor<false>& that)
				WISH_REQUIRES(Const) :
			n_(that.n_),
			end_(that.end_),
			stride_end_(that.stride_end_) {}

		constexpr auto read(const ranges::iterator_t<CRng>& it) const {
			RANGES_EXPECT(it != stride_end_);
			return ranges::view::stride(ranges::make_subrange(it, end_), n_);
		}
	};

	struct sentinel_adaptor : ranges::adaptor_base {
		template <bool Const>
		bool empty(const ranges::iterator_t<Rng>& it, const adaptor<Const>& ia, const ranges::sentinel_t<Rng>& s) const {
			(void) s;
			return it == ia.stride_end_;
		}
	};

private:
	ranges::range_difference_type_t<Rng> n_ = 0;

private:
	constexpr adaptor<ranges::simple_view<Rng>()> begin_adaptor() {
		return adaptor<ranges::simple_view<Rng>()>{*this};
	}

	constexpr adaptor<true> begin_adaptor() const
			WISH_REQUIRES(ranges::random_access_range<const Rng>) {
		return adaptor<true>{*this};
	}

	constexpr auto end_adaptor() const {
		return sentinel_adaptor{};
	}

public:
	deinterleave_view() = default;

	constexpr deinterleave_view(Rng rng, ranges::range_difference_type_t<Rng> n) :
		deinterleave_view::view_adaptor(ranges::detail::move(rng)),
		n_((RANGES_EXPECT(0 < n), n)) { }

	constexpr ranges::range_size_type_t<Rng> size() const
			WISH_REQUIRES(ranges::sized_range<const Rng>) {
		return static_cast<ranges::range_size_type_t<Rng>>(std::min(n_, ranges::distance(this->base())));
	}

	constexpr ranges::range_size_type_t<Rng> size()
			WISH_REQUIRES(ranges::sized_range<Rng>) {
		return static_cast<ranges::range_size_type_t<Rng>>(std::min(n_, ranges::distance(this->base())));
	}
};

} // namespace detail ------------------------------------------------------------------------------

//// In:  Range<T>
//// Out: Range<Range<T>>, where each inner range has $n$ or $n-1$ elements.

struct deinterleave_base_fn {
	template<typename Rng, typename Int>
	constexpr auto operator()(Rng&& rng, Int n) const {
		static_assert(ranges::random_access_range<Rng>, "The range passed to view::deinterleave must be a random access range");
		static_assert(ranges::integral<Int>, "The number passed to view::deinterleave must be an Integral");

		return detail::deinterleave_view<ranges::view::all_t<Rng>>{
			ranges::view::all(static_cast<Rng&&>(rng)), n
		};
	}
};

struct deinterleave_fn : deinterleave_base_fn {
	using deinterleave_base_fn::operator();

	template<typename Int>
	constexpr auto operator()(Int n) const {
		static_assert(ranges::integral<Int>, "The number passed to view::deinterleave must be an Integral");

		return ranges::make_view_closure(ranges::bind_back(deinterleave_base_fn{}, n));
	}
};

/// \relates deinterleave_fn
static constexpr ranges::view::view_closure<deinterleave_fn> deinterleave{};

} // namespace view
} // namespace libv

#include <range/v3/detail/satisfy_boost_range.hpp>
RANGES_SATISFY_BOOST_RANGE(::libv::view::detail::deinterleave_view)
