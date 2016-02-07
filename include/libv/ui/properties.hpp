// File: LayoutInfo.hpp, Created on 2014. április 2. 14:53, Author: Vader

#pragma once

// ext
#include <boost/container/flat_map.hpp>
// libv
#include <libv/property_map.hpp>
#include <libv/vec.hpp>

namespace libv {
namespace ui {

using PropertyMap = BasicPropertyMap<::boost::container::flat_map>;

namespace Property {

extern PropertyMap::Address<bool> NewLine;
extern PropertyMap::Address<vec3> Position;
extern PropertyMap::Address<vec3> Size;
extern PropertyMap::Address<vec4> Border;
extern PropertyMap::Address<vec4> Margin;
extern PropertyMap::Address<vec4> Pedding;

} //namespace Property

} //namespace ui
} //namespace libv