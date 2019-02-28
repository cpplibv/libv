// File:   property.hpp Author: Vader Created on 2019. február 15., 1:32

#pragma once

// std
#include <variant>
// pro
#include <libv/ui/chrono.hpp>
#include <libv/ui/size.hpp>
#include <libv/ui/property/anchor.hpp>
#include <libv/ui/property/color.hpp>
#include <libv/ui/property/font_size.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Font2D;
using Font2D_view = std::shared_ptr<Font2D>;

using PropertyValue = std::variant<
		Anchor,
		Font2D_view,
		FontSize,
		Color,
		Size

//		bool,
//		int32_t,
//		float,
//		double,
//		std::string,
//		libv::vec2i,
//		libv::vec3i,
//		libv::vec4i,
//		libv::vec2f,
//		libv::vec3f,
//		libv::vec4f,
//		libv::vec2d,
//		libv::vec3d,
//		libv::vec4d,
//		time_duration,

//		SizeContent,??
//		SizePercent,??
//		SizePixel,??
//		SizeRatio,??

//		image2D?
//		font2D?
//		layout?
>;

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
