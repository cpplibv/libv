// Project: libv.utility, File: src/libv/utility/type_key.hpp

#pragma once

// libv
#include <libv/utility/type_uid.hpp>
// std
#include <bit>
#include <typeindex>


namespace libv {

// -------------------------------------------------------------------------------------------------

// The std performance for RTTI based hashing is really suboptimal, with this type_key this can be avoided
//		(My guess is that shared linking could introduce a second type info object for the same type, so true name string comparison is required)
// Usage: Use uintptr_t (libv::type_uid) as the key, and use this helper function to create a key from a type
// Performance analysis:
//		https://godbolt.org/z/Kfexxh4f4
//		https://quick-bench.com/q/-8xyfuQaWaG5-mYHGuhXoSL0I3w

template <typename T>
[[nodiscard]] inline libv::type_uid type_key() noexcept {
	return std::bit_cast<libv::type_uid>(std::type_index(typeid(T)));
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
