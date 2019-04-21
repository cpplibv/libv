// File:   property.hpp Author: Vader Created on 2019. február 15., 1:32

#pragma once

// std
#include <variant>
// pro
//#include <libv/ui/chrono.hpp>
#include <libv/ui/parse/parse_size.hpp>
#include <libv/ui/property/align.hpp>
#include <libv/ui/property/color.hpp>
#include <libv/ui/property/font_size.hpp>
#include <libv/ui/property/size.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Font2D;
using Font2D_view = std::shared_ptr<Font2D>;

using PropertyDynamic = std::variant<
		AlignHorizontal,
		AlignVertical,
		Color,
		Font2D_view,
		FontSize,
		Size
>;

////		bool,
////		int32_t,
////		float,
////		double,
////		std::string,
////		libv::vec2i,
////		libv::vec3i,
////		libv::vec4i,
////		libv::vec2f,
////		libv::vec3f,
////		libv::vec4f,
////		libv::vec2d,
////		libv::vec3d,
////		libv::vec4d,
////		time_duration,
//
////		image2D?
////		layout?
//>;

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
