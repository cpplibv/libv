// Project: libv.utility, File: src/libv/utility/bit_cast.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <cstring>
#include <type_traits>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <class Target, class Source>
[[nodiscard]] constexpr inline Target bit_cast(const Source& source) noexcept {
	static_assert(sizeof (Target) == sizeof (Source), "bit_cast the sizeof Source and the Target has to be equal");
	static_assert(std::is_trivially_copyable_v<Target>, "Target has to be trivially_copyable");
	static_assert(std::is_trivially_copyable_v<Source>, "Source has to be trivially_copyable");
	Target dest;
	std::memcpy(&dest, &source, sizeof (dest));
	return dest;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
