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
using PropertyAddress = PropertyMap::Address<T>;

namespace Property {

// Types -------------------------------------------------------------------------------------------

using SizeType = vec3;

// -------------------------------------------------------------------------------------------------

/// The preferred size of the component
extern PropertyAddress<SizeType> Size;
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

// -------------------------------------------------------------------------------------------------

} //namespace Property

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv









// =================================================================================================
// =================================================================================================
// =================================================================================================
//
//// -------------------------------------------------------------------------------------------------
//
//using PropertyMap = BasicPropertyMap<::boost::container::flat_map>;
//template <typename T>
//using PropertyAddress = PropertyMap::Address<T>;
//
//namespace Property {
//
////enum class LayoutType {
////	Element, // Position the component before laying it out. Expand to container
////	Block // Layout the component before positioning it. Shrink to content
////};
//
////using Align = size_t;
////using Orient = size_t;
//
////enum class SizeUnit {
////	//	dot, /// Pixel value scaled by dpi
////
////	expand, /// Expand to container, trying to maximize its size
////	shrink, /// Shrink to content, trying to minimize its size
////
////	//	maxpx,
////	//	minpx,
////	px, /// Plain old pixel
////
////	//	maxpercent,
////	//	minpercent,
////	//	percent,
////
////	//	maxratio,
////	//	minratio,
////	//	ratio
////};
////
////struct SizeDimension {
////	float value;
////	SizeUnit type;
////
////	SizeValue(float value, SizeUnit type = SizeUnit::px) : value(value), type(type) { }
////};
////
////struct SizeType {
////	vec3_t<SizeDimension> value;
////
////	SizeValue(vec3 value, SizeUnit type = SizeUnit::px) :
////		value(SizeDimension(value.x, type), SizeDimension(value.y, type), SizeDimension(value.z, type)) { }
////};
//
//using SizeType = vec3;
//
//// -------------------------------------------------------------------------------------------------
//
///// The layout type
//extern PropertyAddress<LayoutType> Layout;
///// The preferred size of the component
//extern PropertyAddress<SizeType> Size;
//
//// -------------------------------------------------------------------------------------------------
//
/////// The alignment of the contained elements
////extern PropertyAddress<Align> Alignment;
/////// The alignment of the content's bounding box inside the component's bounding box
////extern PropertyAddress<Align> AlignmentContent;
/////// The orientation of the content. "Line direction"
////extern PropertyAddress<Orient> Orientation;
/////// After the component a new line char is interpreted
////extern PropertyAddress<bool> NewLine;
/////// Relative position to the parent
////extern PropertyAddress<vec3> Position;
////extern PropertyAddress<vec4> Border;
////extern PropertyAddress<vec4> Margin;
////extern PropertyAddress<vec4> Pedding;
//
//} //namespace Property
//
//// -------------------------------------------------------------------------------------------------
//
//} //namespace ui
//} //namespace libv
//
//// -------------------------------------------------------------------------------------------------
//
////CSS display
//// block 	Displays an element as a block element (like <p>)
//// flex 	Displays an element as an block-level flex container. New in CSS3
//// inline-block 	Displays an element as an inline-level block container. The inside of this block is formatted as block-level box, and the element itself is formatted as an inline-level box
//// inline-flex 	Displays an element as an inline-level flex container. New in CSS3
//// inline-table 	The element is displayed as an inline-level table
//// list-item 	Let the element behave like a <li> element
//// run-in 	Displays an element as either block or inline, depending on context
//// table 	Let the element behave like a <table> element
//// table-caption 	Let the element behave like a <caption> element
//// table-column-group 	Let the element behave like a <colgroup> element
//// table-header-group 	Let the element behave like a <thead> element
//// table-footer-group 	Let the element behave like a <tfoot> element
//// table-row-group 	Let the element behave like a <tbody> element
//// table-cell 	Let the element behave like a <td> element
//// table-column 	Let the element behave like a <col> element
//// table-row 	Let the element behave like a <tr> element
//// none 	The element will not be displayed at all (has no effect on layout)
//// initial 	Sets this property to its default value. Read about initial
//// inherit Inherits this property from its parent element.