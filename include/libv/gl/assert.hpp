// File: Log.hpp, Created on 2015.02.02. 5:11, Author: Vader

#pragma once

// std
#include <cassert>


// -------------------------------------------------------------------------------------------------

#define LIBV_GL_DEBUG_ASSERT(X) assert((X))
#define LIBV_GL_DEBUG_ASSERT_STATIC(X, M) static_assert((X), M)

// -------------------------------------------------------------------------------------------------
