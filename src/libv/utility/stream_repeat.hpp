// File: stream_repeat.hpp Author: Vader Created on 2017. augusztus 4., 14:22

#pragma once


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
struct StreamRepeat {
	const T& object;
	size_t count;

	StreamRepeat(const T& object, size_t count) : object(object), count(count) {}

	template <typename OStream>
	friend OStream& operator<<(OStream& os, const StreamRepeat<T>& repeater) {
		for (size_t i = 0; i < repeater.count; ++i) {
			os << repeater.object;
		}
		return os;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv