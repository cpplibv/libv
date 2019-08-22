// File:   erase_if_unstable.hpp Author: Vader Created on 22 August 2019, 17:23

#pragma once

// std
#include <utility>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename Container, typename Pred>
constexpr inline void erase_if_unstable(Container& container, const Pred& pred) noexcept {
	auto it = container.begin();
	auto last = container.end();

	while (it != last) {
		auto& element = *it;
		if (pred(element)) {
			--last;
			// Avoiding self assignment
			if (it != last)
				element = std::move(*last);
		} else {
			++it;
		}
	}

	container.erase(last, container.end());
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
