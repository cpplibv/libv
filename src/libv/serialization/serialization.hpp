// Project: libv.serialization, File: src/libv/serialization/serialization.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <cereal/cereal.hpp>
// pro
#include <libv/serialization/enable.hpp>


#define LIBV_NVP(NAME) CEREAL_NVP(NAME)
#define LIBV_NVP_NAMED(NAME, VAR) ::cereal::make_nvp(NAME, VAR)

#define LIBV_SERIALIZATION_EXTENSION_NAMESPACE cereal
