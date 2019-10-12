// File:   erase_if_unstable.hpp Author: Vader Created on 22 August 2019, 17:23

#pragma once

// std
#include <functional>
#include <iterator>
#include <utility>


namespace libv {

// -------------------------------------------------------------------------------------------------

///
template <typename Container>
constexpr inline void erase_all_unstable(Container& container, const typename Container::value_type& value) noexcept {
	auto it = container.begin();
	auto last = container.end();

	while (it != last) {
		auto& element = *it;
		if (element == value) {
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

///
template <typename Container, typename Proj>
constexpr inline void erase_all_unstable(Container& container, const std::invoke_result_t<Proj, typename Container::value_type>& value, const Proj& proj) noexcept {
	auto it = container.begin();
	auto last = container.end();

	while (it != last) {
		auto& element = *it;
		if (std::invoke(proj, element) == value) {
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
