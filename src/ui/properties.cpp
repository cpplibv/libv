// File: Properties.cpp, Created on 2014. április 24. 13:35, Author: Vader

// hpp
#include <libv/ui/properties.hpp>

namespace libv {
namespace ui {

namespace Property {

PropertyAddress<LayoutType> Layout;
PropertyAddress<bool> NewLine;
PropertyAddress<vec3> Position;
PropertyAddress<SizeType> Size;
PropertyAddress<vec4> Border;
PropertyAddress<vec4> Margin;
PropertyAddress<vec4> Pedding;

} //namespace Property

} //namespace ui
} //namespace libv