// File: LayoutInfo.hpp, Created on 2014. április 2. 14:53, Author: Vader

#pragma once

// ext
#include <boost/container/flat_map.hpp>
// libv
#include <libv/varidic_map.hpp>
#include <libv/vec.hpp>

namespace libv {
namespace ui {

using PropertyMap = VaridicMap<::boost::container::flat_map>;

namespace Property {

extern PropertyMap::Address<ivec3> Position;
extern PropertyMap::Address<ivec3> Size;
extern PropertyMap::Address<ivec4> Border;
extern PropertyMap::Address<ivec4> Margin;
extern PropertyMap::Address<ivec4> Pedding;

} //namespace Property

} //namespace ui
} //namespace libv