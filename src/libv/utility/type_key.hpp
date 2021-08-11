// Project: libv.utility, File: src/libv/utility/type_key.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/utility/bit_cast.hpp>
#include <libv/utility/type_uid.hpp>
// std
#include <typeindex>


namespace libv {

// -------------------------------------------------------------------------------------------------

// The std performance for RTTI based hashing is really suboptimal, with this type_key this can be avoided
// Usage: Use uintptr_t as the key, and use this helper function to create a key from a type
// Performance analysis:
//		https://godbolt.org/z/Kfexxh4f4
//		https://quick-bench.com/q/-8xyfuQaWaG5-mYHGuhXoSL0I3w

template <typename T>
[[nodiscard]] inline libv::type_uid type_key() noexcept {
	return libv::bit_cast<libv::type_uid>(std::type_index(typeid(T)));
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
