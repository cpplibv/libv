// Project: libv.utility, File: src/libv/utility/stream_repeat.hpp

#pragma once


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
struct StreamRepeat {
	const T& object;
	std::size_t count;

	StreamRepeat(const T& object, std::size_t count) : object(object), count(count) {}

	template <typename OStream>
	friend OStream& operator<<(OStream& os, const StreamRepeat<T>& repeater) {
		for (std::size_t i = 0; i < repeater.count; ++i) {
			os << repeater.object;
		}
		return os;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv