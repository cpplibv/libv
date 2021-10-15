// Project: libv.ctrl, File: src/libv/ctrl/origin.hpp

#pragma once


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

enum class Origin {
	impulse,

	time,

	mouse,
	scroll,
	gp_analog,
	js_analog,
};

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
