// Project: libv.ctrl, File: src/libv/ctrl/feature.hpp, Author: Császár Mátyás [Vader]

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
