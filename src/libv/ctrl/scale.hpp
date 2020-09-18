// Project: libv.ctrl, File: src/libv/ctrl/scale.hpp, Author: Császár Mátyás [Vader]

#pragma once


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

using analog_type = double;
using scale_type = double;

static constexpr scale_type MOUSE_NORMALIZATION_SCALE = scale_type{1} / scale_type{600};

struct scale_group {
	scale_type impulse = 1;
	scale_type scale = 1;
	scale_type time = 1;
};

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
