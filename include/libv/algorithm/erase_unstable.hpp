// File:   erase_if_unstable.hpp Author: Vader Created on 22 August 2019, 17:23

#pragma once

// std
#include <iterator>
#include <utility>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// The iterator \c it must be valid and dereferenceable. Thus the end() iterator (which is valid, but is not dereferenceable) cannot be used as a value for \c it.
template <typename Container>
constexpr inline void erase_unstable(Container& container, typename Container::iterator it) noexcept {
	const auto last = std::prev(container.end());

	// Avoiding self assignment
	if (it != last)
		*it = std::move(*last);

	container.erase(last);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
