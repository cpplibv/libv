// Project: libv.utility, File: src/libv/utility/predicate.hpp

#pragma once

// std
#include <functional>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <std::size_t N, typename Pred>
auto on_nth(Pred&& pred) {
	return [pred = std::forward<Pred>(pred)](auto&& arg) {
		return pred(std::get<N>(std::forward<decltype(arg)>(arg)));
	};
}

template <typename Pred>
auto on_first(Pred&& pred) {
	return on_nth<0>(std::forward<Pred>(pred));
}

template <typename Pred>
auto on_second(Pred&& pred) {
	return on_nth<1>(std::forward<Pred>(pred));
}

// -------------------------------------------------------------------------------------------------

auto equal_to() {
	return std::equal_to<void>{};
}

template <typename T>
auto equal_to(T&& value) {
	return [value = std::forward<T>(value)](auto&& arg) {
		return value == std::forward<decltype(arg)>(arg);
	};
}

auto not_equal_to() {
	return std::not_equal_to<void>{};
}

template <typename T>
auto not_equal_to(T&& value) {
	return [value = std::forward<T>(value)](auto&& arg) {
		return value != std::forward<decltype(arg)>(arg);
	};
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
