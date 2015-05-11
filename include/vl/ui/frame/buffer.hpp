// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

#pragma once

// vl
#include <vl/context.hpp>
#include <vl/timer.hpp>
// std
#include <queue>

namespace vl {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct ResourceManager {
	std::priority_queue<ResourceTask> io_que; //static....
	std::priority_queue<ResourceTask> gl_que;

	vl::Context* io_context;
	vl::Context* gl_context;
};

struct  {
	
};

// Context Task Priority levels for each operation
//
//	50 Render But based on time, measure it ?
//
//		110 *Unload Texture
//		120 *Unload Model
//	130 Unload UITexture
//	140 Unload Font
//		150 Unload CSS
//		160 Unload Layout
//	170 Unload DefaultFont
//	180 Unload Shader
//	210 Shader
//	220 DefaultFont
//		230 Layout
//		240 CSS
//	250 Font
//	260 UITexture
//		270 *Model
//		280 *Texture

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace vl
