// Project: libv.serial, File: src/libv/serial/size_tag.hpp

#pragma once

// ext
#include <cereal/size_tag.hpp>
#include <cereal/macros.hpp>


namespace libv {
namespace serial {

// -------------------------------------------------------------------------------------------------

//template <typename T>
//using SizeTag = cereal::SizeTag<T>;

using cereal::SizeTag;
using cereal::make_size_tag;

using cereal::size_type;

// -------------------------------------------------------------------------------------------------

} // namespace serial
} // namespace libv
