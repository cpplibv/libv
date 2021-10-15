// Project: libv.gl, File: src/libv/gl/assert.hpp

#pragma once

// std
#include <cassert>


// -------------------------------------------------------------------------------------------------

#define LIBV_GL_DEBUG_ASSERT(X) assert((X))
#define LIBV_GL_DEBUG_ASSERT_STATIC(X, M) static_assert((X), M)

// -------------------------------------------------------------------------------------------------
