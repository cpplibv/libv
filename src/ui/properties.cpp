// File: Properties.cpp, Created on 2014. április 24. 13:35, Author: Vader

// hpp
#include <libv/ui/properties.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

namespace Property {

PropertyAddress<LayoutID>		ID("id", "");
PropertyAddress<LayoutClass>	Class("class", "");
PropertyAddress<LayoutSizeVec>	Size("size", LayoutSizeVec(px(100), px(100), 0));
PropertyAddress<LayoutAlign>	Align("align", 0);
PropertyAddress<LayoutAlign>	Anchor("content", 0);
PropertyAddress<LayoutOrient>	Orient("orient", 0);

} //namespace Property

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv