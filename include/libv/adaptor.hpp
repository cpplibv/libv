// File:   adaptor.hpp Author: Vader Created on 2016. június 1., 5:04

#pragma once

// ext
#include <boost/range/concepts.hpp>
#include <boost/range/iterator_range.hpp>
// std
#include <iterator>

namespace libv {
namespace detail {

// -------------------------------------------------------------------------------------------------

// TODO P5: reversed_if adaptor degrades iterators from random_access to bidirectional

template <typename Iter>
struct reversed_if_iterator : std::iterator<std::bidirectional_iterator_tag, typename Iter::value_type> {
private:
	Iter iter;
	bool reversed;

public:
	reversed_if_iterator(const Iter& iter, bool reversed) : iter(iter), reversed(reversed) { }

	using this_type = reversed_if_iterator<Iter>;
	using value_type = typename Iter::value_type;
	using pointer = typename Iter::pointer;
	using reference = typename Iter::reference;

	reference operator*() const{
		return *iter;
	}
	pointer operator->() const{
		return &*iter;
	}

	this_type& operator++() {
		if (reversed)
			--iter;
		else
			++iter;
		return *this;
	}
	this_type operator++(int) {
		if (reversed)
			return this_type(iter--, reversed);
		else
			return this_type(iter++, reversed);
	}
	this_type& operator--() {
		if (reversed)
			++iter;
		else
			--iter;
		return *this;
	}
	this_type operator--(int) {
		if (reversed)
			return this_type(iter++, reversed);
		else
			return this_type(iter--, reversed);
	}

	bool operator==(const this_type& rhs) const {
		return iter == rhs.iter;
	}
	bool operator!=(const this_type& rhs) const {
		return iter != rhs.iter;
	}
};

template <typename Range>
struct reversed_if_range : public boost::iterator_range<
			reversed_if_iterator<typename boost::range_iterator<Range>::type>> {
private:
	using base_type = boost::iterator_range<
			reversed_if_iterator<typename boost::range_iterator<Range>::type>>;
public:
	using iterator = reversed_if_iterator<typename boost::range_iterator<Range>::type>;

public:
	reversed_if_range(Range& r, bool reversed) :
		base_type(reversed ? iterator(--std::end(r), reversed) : iterator(std::begin(r), reversed),
				reversed ? iterator(--std::begin(r), reversed) : iterator(std::end(r), reversed)) {
	}
};

struct reverse_if_forwarder {
	bool reversed;
};

template <typename BireversedectionalRange>
inline reversed_if_range<BireversedectionalRange>
operator|(BireversedectionalRange& r, reverse_if_forwarder forwarder) {
	BOOST_RANGE_CONCEPT_ASSERT((boost::BidirectionalRangeConcept<BireversedectionalRange>));

	return reversed_if_range<BireversedectionalRange>(r, forwarder.reversed);
}

template <typename BireversedectionalRange>
inline reversed_if_range<const BireversedectionalRange>
operator|(const BireversedectionalRange& r, reverse_if_forwarder forwarder) {
	BOOST_RANGE_CONCEPT_ASSERT((boost::BidirectionalRangeConcept<const BireversedectionalRange>));

	return reversed_if_range<const BireversedectionalRange>(r, forwarder.reversed);
}

} //namespace detail

// -------------------------------------------------------------------------------------------------

// TODO P5: move reverse_if into new range lib view namespace
const detail::reverse_if_forwarder reverse_if(bool reversed = false) {
	return detail::reverse_if_forwarder{reversed};
}

template <typename BireversedectionalRange>
inline detail::reversed_if_range<BireversedectionalRange>
reverse_if(BireversedectionalRange& rng, bool reversed = false) {
	BOOST_RANGE_CONCEPT_ASSERT((boost::BidirectionalRangeConcept<BireversedectionalRange>));

	return detail::reversed_if_range<BireversedectionalRange>(rng, reversed);
}

template <typename BireversedectionalRange>
inline detail::reversed_if_range<const BireversedectionalRange>
reverse_if(const BireversedectionalRange& rng, bool reversed = false) {
	BOOST_RANGE_CONCEPT_ASSERT((boost::BidirectionalRangeConcept<const BireversedectionalRange>));

	return detail::reversed_if_range<const BireversedectionalRange>(rng, reversed);
}

// -------------------------------------------------------------------------------------------------

} //namespace libv