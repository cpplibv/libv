// Project: libv.utility, File: src/libv/utility/screen_picker.hpp

#pragma once

// libv
#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
class screen_picker {
	using float_type = T;
	using mat4 = libv::mat4_t<float_type>;
	using vec2 = libv::vec2_t<float_type>;
	using vec3 = libv::vec3_t<float_type>;
	using vec4 = libv::vec4_t<float_type>;

private:
	vec2 canvas_size;
	mat4 matrixPV;
	mat4 inversePV;

public:
	screen_picker(const mat4& matrixPV, vec2 canvas_size) :
		canvas_size(canvas_size),
		matrixPV(matrixPV),
		inversePV(matrixPV.inverse_copy()) { }

	vec3 to_world(const vec2 canvas_coord) const {
		auto coord2Scaled = canvas_coord / canvas_size * float_type{2} - float_type{1};
		auto coord4ScaledStart = vec4(coord2Scaled, float_type{-1}, float_type{1});
		auto coord4ScaledEnd = vec4(coord2Scaled, float_type{0}, float_type{1});

		auto coordLineWStart = inversePV * coord4ScaledStart;
		coordLineWStart /= coordLineWStart.w;

		auto coordLineWEnd = inversePV * coord4ScaledEnd;
		coordLineWEnd /= coordLineWEnd.w;

		return libv::vec::xyz((coordLineWEnd - coordLineWStart).normalize());
	}

	vec2 to_screen(const vec3 worldCoord) const {
		auto objectSPosition = matrixPV * vec4(worldCoord, float_type{1});

//		if (objectSPosition.w <= 0)
//			// Point not on screen

		objectSPosition /= objectSPosition.w;

		return (libv::vec::xy(objectSPosition) + float_type{1}) * float_type{0.5} * canvas_size;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
