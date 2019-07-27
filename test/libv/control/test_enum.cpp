// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// libv
#include <libv/utility/enum.hpp>
// pro
#include <libv/control/enum.hpp>


// -------------------------------------------------------------------------------------------------

namespace lc = libv::control;

// -------------------------------------------------------------------------------------------------

TEST_CASE("Compatibility assumption checks", "[libv.control]") {
	CHECK(libv::to_value(lc::AnalogDimension::x) == libv::to_value(lc::MouseMovement::x));
	CHECK(libv::to_value(lc::AnalogDimension::x_plus) == libv::to_value(lc::MouseMovement::x_plus));
	CHECK(libv::to_value(lc::AnalogDimension::x_minus) == libv::to_value(lc::MouseMovement::x_minus));
	CHECK(libv::to_value(lc::AnalogDimension::y) == libv::to_value(lc::MouseMovement::y));
	CHECK(libv::to_value(lc::AnalogDimension::y_plus) == libv::to_value(lc::MouseMovement::y_plus));
	CHECK(libv::to_value(lc::AnalogDimension::y_minus) == libv::to_value(lc::MouseMovement::y_minus));

	CHECK(libv::to_value(lc::AnalogDimension::x) == libv::to_value(lc::MouseScroll::x));
	CHECK(libv::to_value(lc::AnalogDimension::x_plus) == libv::to_value(lc::MouseScroll::x_plus));
	CHECK(libv::to_value(lc::AnalogDimension::x_minus) == libv::to_value(lc::MouseScroll::x_minus));
	CHECK(libv::to_value(lc::AnalogDimension::y) == libv::to_value(lc::MouseScroll::y));
	CHECK(libv::to_value(lc::AnalogDimension::y_plus) == libv::to_value(lc::MouseScroll::y_plus));
	CHECK(libv::to_value(lc::AnalogDimension::y_minus) == libv::to_value(lc::MouseScroll::y_minus));
}

// -------------------------------------------------------------------------------------------------
