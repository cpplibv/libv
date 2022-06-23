// Project: libv.serial, File: src/libv/serial/nvp.hpp

#pragma once

// ext
#include <vide/nvp.hpp>


// -------------------------------------------------------------------------------------------------

#define LIBV_NVP(NAME) VIDE_NVP(NAME)
#define LIBV_NVP_NAMED(NAME, VAR) VIDE_NVP_(NAME, VAR)
#define LIBV_NVP_FORCED(NAME, VAR) ::vide::make_nvp(NAME, VAR)

// -------------------------------------------------------------------------------------------------
