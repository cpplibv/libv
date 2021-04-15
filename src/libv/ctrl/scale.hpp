// Project: libv.ctrl, File: src/libv/ctrl/scale.hpp, Author: Császár Mátyás [Vader]

#pragma once


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

using analog_type = double;
using scale_type = double;

// TODO P4: Merge in scale_2d.hpp (if vec include is happening either way by an other header)
//using scale_type_2D = libv::vec2_t<scale_type>;
//using analog_type_2D = libv::vec2_t<analog_type>;

struct scale_group {
	scale_type impulse = 1;

	scale_type time = 1;

	scale_type mouse = 1;
	scale_type scroll = 1;
	scale_type gp_analog = 1;
	scale_type js_analog = 1;
};

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
