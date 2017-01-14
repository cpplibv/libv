// File: Config.hpp, Created on 2014. november 30. 13:58, Author: Vader

#pragma once

#include <cassert>

// -------------------------------------------------------------------------------------------------

// TODO P4: include xy log and check (for checkGL)
// TODO P4: LIBV_UI_* -> LIBV_LOG_UI_*
// TODO P4: LIBV_UI_DEBUG_ -> LIBV_UI_DEBUG
// TODO P5: find a good place for these assert functions something something assert.hpp

#ifndef LIBV_UI_DEBUG
#    define LIBV_UI_DEBUG // TODO P5: debug flag
#endif

#ifdef LIBV_UI_DEBUG
#    define LIBV_UI_DEBUG_ASSERT(X) assert((X))
#    define LIBV_UI_DEBUG_ASSERT_CONTEXT(X) {} // TODO P5: assert context
#    define LIBV_UI_DEBUG_ASSERT_STATIC(X, M) static_assert((X), (M))
#    define LIBV_UI_DEBUG_ASSERT_THREAD(X) {} // TODO P5: assert thread
#    define LIBV_UI_DEBUG_CHECK_GL() checkGL()
#else
#    define LIBV_UI_DEBUG_ASSERT(X) {}
#    define LIBV_UI_DEBUG_ASSERT_CONTEXT(X) {} // TODO P5: assert context
#    define LIBV_UI_DEBUG_ASSERT_STATIC(X) {}
#    define LIBV_UI_DEBUG_ASSERT_THREAD(X) {} // TODO P5: assert thread
#    define LIBV_UI_DEBUG_CHECK_GL() {}
#endif

#define LIBV_UI_ASSERT_CONTEXT(X) {} // TODO P5: assert context
#define LIBV_UI_ASSERT_THREAD(X) {} // TODO P5: assert thread

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

constexpr unsigned int DEFAULT_FRAME_WIDTH = 640;
constexpr unsigned int DEFAULT_FRAME_HEIGHT = 480;

constexpr const char DEFAULT_FONT_TTF_FILEPATH[] = "res/font/default.ttf";

//constexpr const char FOLDER_SHADER[] = "res/shader/";

constexpr const char DEFAULT_PROGRAM_FILEPATH[] = "res/shader/ui/default";

//constexpr const char DEFAULT_PROGRAM_VS_FILEPATH[] = "res/shader/ui/default.vs";
//constexpr const char DEFAULT_PROGRAM_FS_FILEPATH[] = "res/shader/ui/default.fs";

constexpr const char DEFAULT_FONT_VS_FILEPATH[] = "res/shader/font.vs";
constexpr const char DEFAULT_FONT_FS_FILEPATH[] = "res/shader/font.fs";

// -------------------------------------------------------------------------------------------------

enum class ThreadAffinity {
	ANY,
	GL,
	IO,
	IN,
};

enum class ResourceStepResult {
	OK = 0,
	//	Abort, //?
	//	Rollback, //?
	Failed,
};

enum class ResourceState {
	OK = 0,
	//	Aborted,
	Failed,
};

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv
