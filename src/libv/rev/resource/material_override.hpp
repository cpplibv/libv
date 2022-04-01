// Project: libv.rev, File: src/libv/rev/resource/material_override.hpp

#pragma once

// libv
#include <libv/vm4/fwd.hpp>
#include <libv/utility/unique_function.hpp>
// pro
#include <libv/rev/fwd.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

using MaterialScanFunc = libv::unique_function<Material(const MaterialScanner&) const>;
using MaterialLoaderFunc = libv::unique_function<Material(const libv::vm4::Material&) const>;

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
