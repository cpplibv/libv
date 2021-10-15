// Project: libv.glr, File: src/libv/glr/assert.hpp

#pragma once

// std
#include <cassert>


// -------------------------------------------------------------------------------------------------

#define LIBV_GLR_DEBUG_ASSERT(X) assert((X))
#define LIBV_GLR_DEBUG_ASSERT_STATIC(X, M) static_assert((X), M)

// -------------------------------------------------------------------------------------------------
