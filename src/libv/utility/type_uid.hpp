// Project: libv.utility, File: src/libv/utility/type_uid.hpp

#pragma once

// std
#include <cstdint>


namespace libv {

// -------------------------------------------------------------------------------------------------

// The std performance for RTTI based hashing is really suboptimal, with this type_key this can be avoided
//		(My guess is that shared linking could introduce a second type info object for the same type, so true name string comparison is required)
// Performance analysis:
//		https://godbolt.org/z/Kfexxh4f4
//		https://quick-bench.com/q/-8xyfuQaWaG5-mYHGuhXoSL0I3w

using type_uid = uintptr_t;

// -------------------------------------------------------------------------------------------------

} // namespace libv
