// Project: libv.algo, File: src/libv/algo/erase_unstable.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <iterator>
#include <utility>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// The iterator \c it must be valid and dereferenceable. Thus the end() iterator (which is valid, but is not dereferenceable) cannot be used as a value for \c it.
template <typename Container>
constexpr inline void erase_unstable(Container& container, typename Container::iterator it) noexcept {
	const auto last = container.begin() + container.size() - 1;

	// Avoiding self assignment
	if (it != last)
		*it = std::move(*last);

	container.erase(last);
}

template <typename Container>
constexpr inline void erase_unstable(Container& container, const typename Container::value_type& value) noexcept {
	const auto it = std::find(container.begin(), container.end(), value);

	if (it == container.end())
		return;

	erase_unstable(container, it);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
