// File: LayoutInfo.hpp, Created on 2014. április 2. 14:53, Author: Vader

#pragma once

// ext
#include <boost/container/flat_map.hpp>
// libv
#include <libv/property_map.hpp>
#include <libv/vec.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

using PropertyMap = BasicPropertyMap<::boost::container::flat_map>;
template <typename T>
using PropertyAddress = PropertyMap::Key<T>;

// LayoutSize --------------------------------------------------------------------------------------

struct LayoutSize {
	using Unit = uint8_t;

	static constexpr Unit PX = 0;
	static constexpr Unit PERCENT = 1;
	static constexpr Unit NOSPEC = 2;
	// static constexpr Type RATIO = 0;
public:
	float value;
	Unit type;

public:
	LayoutSize(float value = 0.0f, Unit type = PX) : value(value), type(type) { }
};
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

// Property ----------------------------------------------------------------------------------------

namespace Property {

/// The preferred size of the component
extern PropertyAddress<LayoutSizeVec> Size;

///// The alignment of the contained elements
//extern PropertyAddress<Align> Alignment;
///// The alignment of the content's bounding box inside the component's bounding box
//extern PropertyAddress<Align> AlignmentContent;
///// The orientation of the content. "Line direction"
//extern PropertyAddress<Orient> Orientation;
///// After the component a new line char is interpreted
//extern PropertyAddress<bool> NewLine;
///// Relative position to the parent
//extern PropertyAddress<vec3> Position;
//extern PropertyAddress<vec4> Border;
//extern PropertyAddress<vec4> Margin;
//extern PropertyAddress<vec4> Pedding;

} //namespace Property

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv