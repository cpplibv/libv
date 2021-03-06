// Project: libv.serial, File: src/libv/serial/serial.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <cereal/cereal.hpp>
// pro
#include <libv/serial/enable.hpp>


// -------------------------------------------------------------------------------------------------

#define LIBV_NVP(NAME) CEREAL_NVP(NAME)
#define LIBV_NVP_NAMED(NAME, VAR) ::cereal::make_nvp(NAME, VAR)

#define LIBV_SERIAL_EXTENSION_NAMESPACE cereal

// -------------------------------------------------------------------------------------------------
