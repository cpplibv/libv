// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]

#pragma once


namespace libv {
namespace control {

// -------------------------------------------------------------------------------------------------

using analog_type = double;
using scale_type = double;

static constexpr scale_type MOUSE_NORMALIZATION_SCALE = scale_type{1} / scale_type{600};

// -------------------------------------------------------------------------------------------------

} //namespace control
} //namespace libv
