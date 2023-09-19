// Project: libv.re, File: src/libv/re/resource/material_func.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/utility/unique_function.hpp>
#include <libv/vm4/fwd.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

using MaterialScanFunc = libv::unique_function<Material_ptr(const MaterialScanner&) const>;
using MaterialLoaderFunc = libv::unique_function<Material_ptr(const libv::vm4::Material&) const>;

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
