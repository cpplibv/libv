// File: Config.hpp, Created on 2014. november 30. 13:58, Author: Vader

#pragma once

#include <cassert>

// -------------------------------------------------------------------------------------------------

// TODO P4: include xy log and check (for LIBV_GL_CHECK)
// TODO P4: LIBV_FRAME_* -> LIBV_LOG_FRAME_*
// TODO P4: LIBV_FRAME_DEBUG_ -> LIBV_FRAME_DEBUG
// TODO P5: find a good place for these assert functions something something assert.hpp

#ifndef LIBV_FRAME_DEBUG
#    define LIBV_FRAME_DEBUG // TODO P5: debug flag
#endif

#ifdef LIBV_FRAME_DEBUG
#    define LIBV_FRAME_DEBUG_ASSERT(X) assert((X))
#    define LIBV_FRAME_DEBUG_ASSERT_CONTEXT(X) {} // TODO P5: assert context
#    define LIBV_FRAME_DEBUG_ASSERT_STATIC(X, M) static_assert((X), (M))
#    define LIBV_FRAME_DEBUG_ASSERT_THREAD(X) {} // TODO P5: assert thread
#    define LIBV_FRAME_DEBUG_CHECK_GL() LIBV_GL_CHECK()
#else
#    define LIBV_FRAME_DEBUG_ASSERT(X) {}
#    define LIBV_FRAME_DEBUG_ASSERT_CONTEXT(X) {} // TODO P5: assert context
#    define LIBV_FRAME_DEBUG_ASSERT_STATIC(X) {}
#    define LIBV_FRAME_DEBUG_ASSERT_THREAD(X) {} // TODO P5: assert thread
#    define LIBV_FRAME_DEBUG_CHECK_GL() {}
#endif

#define LIBV_FRAME_ASSERT_CONTEXT(X) {} // TODO P5: assert context
#define LIBV_FRAME_ASSERT_THREAD(X) {} // TODO P5: assert thread

namespace libv {
namespace frame {

// -------------------------------------------------------------------------------------------------

constexpr unsigned int DEFAULT_FRAME_WIDTH = 640;
constexpr unsigned int DEFAULT_FRAME_HEIGHT = 480;

// -------------------------------------------------------------------------------------------------

} // namespace frame
} // namespace libv
