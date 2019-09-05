// File: Config.hpp, Created on 2014. november 30. 13:58, Author: Vader

#pragma once

// std
#include <cassert>


// -------------------------------------------------------------------------------------------------

// TODO P5: find a good place for these assert functions something something assert.hpp
// TODO P5: implement assert context
// TODO P5: implement assert thread

#define LIBV_FRAME_DEBUG_ASSERT(X) assert((X))
#define LIBV_FRAME_DEBUG_ASSERT_CONTEXT(X) {}
#define LIBV_FRAME_DEBUG_ASSERT_STATIC(X, M) static_assert((X), (M))
#define LIBV_FRAME_DEBUG_ASSERT_THREAD(X) {}

#define LIBV_FRAME_ASSERT_CONTEXT(X) {}
#define LIBV_FRAME_ASSERT_THREAD(X) {}
