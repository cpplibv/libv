// File: LayoutInfo.hpp, Created on 2014. április 2. 14:53, Author: Vader

#pragma once

// ext
#include <boost/container/flat_map.hpp>
#include <boost/optional.hpp>
// libv
#include <libv/property_map.hpp>
#include <libv/vec.hpp>

namespace libv {
namespace ui {

using PropertyMap = BasicPropertyMap<::boost::container::flat_map>;
template<typename T>
using PropertyAddress = PropertyMap::Address<::boost::optional<T>>;

namespace Property {

enum class LayoutType {
	Element, // Position the component before laying it out. Expand to container
	Block // Layout the component before positioning it. Shrink to content
};

using SizeType = fvec3;

extern PropertyAddress<LayoutType> Layout;
//extern PropertyAddress<bool> NewLine;    // After the component a new line char is interpreted
//extern PropertyAddress<vec3> Position;   // Relative position to the parent
extern PropertyAddress<SizeType> Size;	  //
//extern PropertyAddress<vec4> Border;	    //
//extern PropertyAddress<vec4> Margin;	    //
//extern PropertyAddress<vec4> Pedding;    //

} //namespace Property

} //namespace ui
} //namespace libv


// -------------------------------------------------------------------------------------------------

//enum class SizeDimensionType {
//	//	dot,
//
//	//	expand,
//	//	shrink,
//
//	//	maxpx,
//	//	minpx,
//	px,
//
//	//	maxpercent,
//	//	minpercent,
//	//	percent,
//
//	//	maxratio,
//	//	minratio,
//	//	ratio
//};
//
//struct SizeDimension {
//	float value;
//	SizeDimensionType type;
//};
//
//using SizeType = vec_t<3, SizeDimension>;

// -------------------------------------------------------------------------------------------------

//CSS display
// block 	Displays an element as a block element (like <p>)
// flex 	Displays an element as an block-level flex container. New in CSS3
// inline-block 	Displays an element as an inline-level block container. The inside of this block is formatted as block-level box, and the element itself is formatted as an inline-level box
// inline-flex 	Displays an element as an inline-level flex container. New in CSS3
// inline-table 	The element is displayed as an inline-level table
// list-item 	Let the element behave like a <li> element
// run-in 	Displays an element as either block or inline, depending on context
// table 	Let the element behave like a <table> element
// table-caption 	Let the element behave like a <caption> element
// table-column-group 	Let the element behave like a <colgroup> element
// table-header-group 	Let the element behave like a <thead> element
// table-footer-group 	Let the element behave like a <tfoot> element
// table-row-group 	Let the element behave like a <tbody> element
// table-cell 	Let the element behave like a <td> element
// table-column 	Let the element behave like a <col> element
// table-row 	Let the element behave like a <tr> element
// none 	The element will not be displayed at all (has no effect on layout)
// initial 	Sets this property to its default value. Read about initial
// inherit Inherits this property from its parent element.