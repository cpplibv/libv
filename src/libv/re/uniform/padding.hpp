// Project: libv.re, File: src/libv/re/uniform/padding.hpp

#pragma once


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

template <uint32_t N>
struct Padding {
	using is_padding = void;
	char _bytes[N]{};
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
