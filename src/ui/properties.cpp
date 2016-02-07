// File: Properties.cpp, Created on 2014. április 24. 13:35, Author: Vader

// hpp
#include <libv/ui/properties.hpp>

namespace libv {
namespace ui {

namespace Property {

PropertyMap::Address<bool> NewLine;
PropertyMap::Address<vec3> Position;
PropertyMap::Address<vec3> Size;
PropertyMap::Address<vec4> Border;
PropertyMap::Address<vec4> Margin;
PropertyMap::Address<vec4> Pedding;

} //namespace Property

} //namespace ui
} //namespace libv