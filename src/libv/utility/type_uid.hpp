// Project: libv.utility, File: src/libv/utility/type_uid.hpp

#pragma once

// std
#include <cstdint>


namespace libv {

// -------------------------------------------------------------------------------------------------

// The std performance for RTTI based hashing is really suboptimal, with this type_key this can be avoided
// Usage: Use uintptr_t as the key, and use this helper function to create a key from a type
// Performance analysis:
//		https://godbolt.org/z/Kfexxh4f4
//		https://quick-bench.com/q/-8xyfuQaWaG5-mYHGuhXoSL0I3w

using type_uid = uintptr_t;

// -------------------------------------------------------------------------------------------------

} // namespace libv
