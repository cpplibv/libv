// Project: libv.algo, File: src/libv/algo/erase_if_unstable.hpp

#pragma once

// std
#include <utility>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Container, typename Pred>
constexpr inline auto remove_if_unstable(Container& container, const Pred& pred) noexcept {
	auto it = container.begin();
	auto last = container.end();

	while (it != last) {
		if (pred(*it)) {
			--last;
			// Avoiding self assignment
			if (it != last)
				std::swap(*it, *last);
		} else {
			++it;
		}
	}

	return last; // Last points to the new 'end', to the first removed element (or the end if there was none)
}

template <typename Container, typename Pred>
constexpr inline void erase_if_unstable(Container& container, const Pred& pred) noexcept {
	auto it = container.begin();
	auto last = container.end();

	while (it != last) {
		if (pred(*it)) {
			--last;
			// Avoiding self assignment
			if (it != last)
				*it = std::move(*last);
		} else {
			++it;
		}
	}

	container.erase(last, container.end());
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
