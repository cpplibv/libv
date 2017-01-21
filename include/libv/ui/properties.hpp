// File: LayoutInfo.hpp, Created on 2014. április 2. 14:53, Author: Vader

#pragma once

// std
#include <string>
// libv
#include <libv/ui/property_map.hpp>
#include <libv/vec.hpp>

namespace libv {
namespace ui {

// LayoutSize --------------------------------------------------------------------------------------

struct LayoutSize {
	using Unit = uint8_t;

	static constexpr Unit PX = 0;
	static constexpr Unit PERCENT = 1;
	static constexpr Unit NOSPEC = 2;
	static constexpr Unit RATIO = 3;
public:
	float value;
	Unit type;

public:
	LayoutSize() : value(0.f), type(NOSPEC) { }
	LayoutSize(float value, Unit type = PX) : value(value), type(type) { }
};
inline LayoutSize ratio(float v) {
	// TODO P3: implement ratio in layout size
	return LayoutSize(v, LayoutSize::RATIO);
}
inline LayoutSize percent(float v) {
	return LayoutSize(v, LayoutSize::PERCENT);
}
inline LayoutSize px(float v) {
	return LayoutSize(v, LayoutSize::PX);
}
inline LayoutSize nospec(float v = 0) {
	return LayoutSize(v, LayoutSize::NOSPEC);
}

using LayoutSizeVec = vec3_t<LayoutSize>;

// LayoutAlign -------------------------------------------------------------------------------------

using LayoutAlign = size_t;

constexpr LayoutAlign ALIGN_BOTTOM_CENTER = 0;
constexpr LayoutAlign ALIGN_BOTTOM_LEFT = 1;
constexpr LayoutAlign ALIGN_BOTTOM_RIGHT = 2;
constexpr LayoutAlign ALIGN_CENTER_CENTER = 3;
constexpr LayoutAlign ALIGN_CENTER_LEFT = 4;
constexpr LayoutAlign ALIGN_CENTER_RIGHT = 5;
constexpr LayoutAlign ALIGN_TOP_CENTER = 6;
constexpr LayoutAlign ALIGN_TOP_LEFT = 7;
constexpr LayoutAlign ALIGN_TOP_RIGHT = 8;

// LayoutOrient ------------------------------------------------------------------------------------

using LayoutOrient = size_t;

constexpr LayoutOrient ORIENT_UP_LEFT = 0;
constexpr LayoutOrient ORIENT_UP_RIGHT = 1;
constexpr LayoutOrient ORIENT_DOWN_LEFT = 2;
constexpr LayoutOrient ORIENT_DOWN_RIGHT = 3;
constexpr LayoutOrient ORIENT_LEFT_UP = 4;
constexpr LayoutOrient ORIENT_LEFT_DOWN = 5;
constexpr LayoutOrient ORIENT_RIGHT_UP = 6;
constexpr LayoutOrient ORIENT_RIGHT_DOWN = 7;

// -------------------------------------------------------------------------------------------------

using LayoutClass = std::string;
using LayoutID = std::string;

// Property ----------------------------------------------------------------------------------------

namespace Property {

/// The ID of the component
extern PropertyAddress<LayoutID> ID;

/// The class of the component
extern PropertyAddress<LayoutClass> Class;

/// The preferred size of the component
extern PropertyAddress<LayoutSizeVec> Size;

/// The alignment of the contained elements
extern PropertyAddress<LayoutAlign> Align;

/// The alignment of the content's bounding box inside the component's bounding box
extern PropertyAddress<LayoutAlign> Anchor;

/// The orientation of the content. "Line direction"
extern PropertyAddress<LayoutOrient> Orient;

///// After the component a new line char is interpreted
//extern PropertyAddress<bool> NewLine;
///// Relative position to the parent
//extern PropertyAddress<vec3> Position;
//extern PropertyAddress<vec4> Border;
//extern PropertyAddress<vec4> Margin;
//extern PropertyAddress<vec4> Pedding;

} // namespace Property

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv