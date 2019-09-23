//// File: utf8.hpp Author: Vader Created on 2017. január 19., 1:07

#pragma once

// ext
#include <meta/meta.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/detail/satisfy_boost_range.hpp>
#include <range/v3/iterator_range.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/stride.hpp>
#include <range/v3/view_adaptor.hpp>


namespace libv {
namespace view {

// -------------------------------------------------------------------------------------------------

namespace detail {

template <typename Rng>
struct deinterleave_view : ranges::view_adaptor<deinterleave_view<Rng>, Rng> {
private:
	friend ranges::range_access;
	CONCEPT_ASSERT(ranges::RandomAccessRange<Rng>());

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

		CONCEPT_REQUIRES(Const)
		constexpr adaptor(adaptor<false> that) :
			n_(that.n_),
			end_(that.end_),
			stride_end_(that.stride_end_) {}

		constexpr auto read(const ranges::iterator_t<CRng>& it) const {
			RANGES_EXPECT(it != stride_end_);
			return ranges::view::stride(ranges::make_iterator_range(it, end_), n_);
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

	CONCEPT_REQUIRES(ranges::RandomAccessRange<const Rng>())
	constexpr adaptor<true> begin_adaptor() const {
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

	CONCEPT_REQUIRES(ranges::SizedRange<const Rng>())
	constexpr ranges::range_size_type_t<Rng> size() const {
		return static_cast<ranges::range_size_type_t<Rng>>(std::min(n_, ranges::distance(this->base())));
	}

	CONCEPT_REQUIRES(ranges::SizedRange<Rng>())
	constexpr ranges::range_size_type_t<Rng> size() {
		return static_cast<ranges::range_size_type_t<Rng>>(std::min(n_, ranges::distance(this->base())));
	}
};

} // namespace detail ------------------------------------------------------------------------------

// In:  Range<T>
// Out: Range<Range<T>>, where each inner range has $n$ or $n-1$ elements.
struct deinterleave_fn {
	friend ranges::view::view_access;

private:
	template<typename Int,
			CONCEPT_REQUIRES_(ranges::Integral<Int>())>
	static auto bind(deinterleave_fn deinterleave, Int n)
		RANGES_DECLTYPE_AUTO_RETURN(ranges::make_pipeable(std::bind(deinterleave, std::placeholders::_1, n)))

public:
	template<typename Rng,
			CONCEPT_REQUIRES_(ranges::RandomAccessRange<Rng>())>
	detail::deinterleave_view<ranges::view::all_t<Rng>> operator()(Rng&& rng, ranges::range_difference_type_t<Rng> n) const {
		return {ranges::view::all(static_cast<Rng&&>(rng)), n};
	}

private:
	template<typename Int,
			CONCEPT_REQUIRES_(!ranges::Integral<Int>())>
	static ranges::detail::null_pipe bind(deinterleave_fn, Int) {
		static_assert(ranges::Integral<Int>(),
				"The object passed to view::deinterleave must be Integral");
		return {};
	}

public:
	template<typename Rng, typename T,
			CONCEPT_REQUIRES_(!(ranges::RandomAccessRange<Rng>() && ranges::Integral<T>()))>
	void operator()(Rng&&, T) const {
		static_assert(ranges::RandomAccessRange<Rng>(),
				"The first argument to view::deinterleave must satisfy the RandomAccessRange concept");
		static_assert(ranges::Integral<T>(),
				"The second argument to view::deinterleave must satisfy the Integral concept");
	}
};

/// \relates deinterleave_fn
static constexpr ranges::view::view<deinterleave_fn> deinterleave{};

} // namespace view
} // namespace libv

RANGES_SATISFY_BOOST_RANGE(::libv::view::detail::deinterleave_view)
