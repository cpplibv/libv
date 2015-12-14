// File: Properties.cpp, Created on 2014. április 24. 13:35, Author: Vader

// hpp
#include <libv/ui/layout/properties.hpp>

namespace libv {
namespace ui {

namespace Property {

PropertyMap::Address<ivec3> Size(50, 50, 0);
PropertyMap::Address<ivec3> Position(0, 0, 0);
PropertyMap::Address<ivec4> Border(0, 0, 0, 0);
PropertyMap::Address<ivec4> Margin(0, 0, 0, 0);
PropertyMap::Address<ivec4> Pedding(0, 0, 0, 0);
//TODO P3: Property defaults per layout manager ?

} //namespace Property

} //namespace ui
} //namespace libv