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

// TODO P4: scale_group2 will kill this MOUSE_NORMALIZATION_SCALE hack
static constexpr scale_type MOUSE_NORMALIZATION_SCALE = scale_type{1} / scale_type{600};

struct scale_group {
	scale_type impulse = 1;
	scale_type scale = 1;
	scale_type time = 1;
};

// TODO P4: scale_group2 split scales by origin
//struct scale_group2 {
//	scale_type impulse = 1;
//	scale_type scale_scroll = 1;
//	scale_type scale_mouse = 1;
//	scale_type scale_gp_analog = 1;
//	scale_type scale_js_analog = 1;
//	scale_type time = 1;
//};

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
