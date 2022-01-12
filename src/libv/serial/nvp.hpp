// Project: libv.serial, File: src/libv/serial/nvp.hpp

#pragma once

// ext
#include <cereal/nvp.hpp>


// -------------------------------------------------------------------------------------------------

#define LIBV_NVP(NAME) CEREAL_NVP(NAME)
#define LIBV_NVP_NAMED(NAME, VAR) CEREAL_NVP_(NAME, VAR)
#define LIBV_NVP_FORCED(NAME, VAR) ::cereal::make_nvp(NAME, VAR)

// -------------------------------------------------------------------------------------------------
