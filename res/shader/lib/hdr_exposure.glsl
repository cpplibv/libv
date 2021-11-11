#pragma once


/// Based on https://learnopengl.com/Advanced-Lighting/HDR
///
///	Exposure value 1.0 does not alters the color.
///	Values above 1.0 makes the color brighter.
///	Values below 1.0 makes the color darker.
///	After hdr_exposure it is recommended to do gamma correction.
///
/// \param - hdr_color vec3(0..inf)
/// \param - exposure (0..inf)
vec3 hdr_exposure(in vec3 hdr_color, in float exposure) {
	return vec3(1.0) - exp(-hdr_color * exposure);
}
