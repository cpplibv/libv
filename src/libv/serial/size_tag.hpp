// Project: libv.serial, File: src/libv/serial/size_tag.hpp

#pragma once

// ext
#include <vide/size_tag.hpp>
#include <vide/macros.hpp>


namespace libv {
namespace serial {

// -------------------------------------------------------------------------------------------------

//template <typename T>
//using SizeTag = vide::SizeTag<T>;

using vide::SizeTag;
using vide::make_size_tag;

using vide::size_type;

// -------------------------------------------------------------------------------------------------

} // namespace serial
} // namespace libv
