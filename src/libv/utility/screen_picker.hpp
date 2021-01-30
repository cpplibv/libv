// Project: libv.utility, File: src/libv/utility/screen_picker.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
class ScreenPicker {
	libv::vec_t<2, T> screenSize;
	libv::mat_t<4, T> matrixPV;
	libv::mat_t<4, T> inversePV;

public:
	ScreenPicker(const libv::mat_t<4, T>& matrixPV, const libv::vec_t<2, T>& screenSize) :
		screenSize(screenSize),
		matrixPV(matrixPV),
		inversePV(matrixPV.inverse_copy()) { }

	libv::vec_t<3, T> to_world(const libv::vec_t<2, T>& screenCoord) const {
		auto coord2Scaled = screenCoord / screenSize * static_cast<T>(2.0) - static_cast<T>(1.0);
		auto coord4ScaledStart = libv::vec_t<4, T>(coord2Scaled, static_cast<T>(-1.0), static_cast<T>(1.0));
		auto coord4ScaledEnd = libv::vec_t<4, T>(coord2Scaled, static_cast<T>(0.0), static_cast<T>(1.0));

		auto coordLineWStart = inversePV * coord4ScaledStart;
		coordLineWStart /= coordLineWStart.w;

		auto coordLineWEnd = inversePV * coord4ScaledEnd;
		coordLineWEnd /= coordLineWEnd.w;

		return libv::vec::xyz((coordLineWEnd - coordLineWStart).normalize());
	}

	libv::vec_t<2, T> to_screen(const libv::vec_t<3, T>& worldCoord) const {
		auto objectSPosition = matrixPV * libv::vec_t<4, T>(worldCoord, static_cast<T>(1.0));
		objectSPosition /= objectSPosition.w;

		return (libv::vec::xy(objectSPosition) + static_cast<T>(1.0)) * static_cast<T>(0.5) * screenSize;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
