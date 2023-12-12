// Project: libv.re, File: src/libv/re/material/material_skybox.hpp

#pragma once

#include <cstdint>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

enum class SkyboxType : uint32_t {
	cubemapXYZ = 0,
	cubemapZXY = 1,
	equirectangular = 2,
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv