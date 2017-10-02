// File: serialization.hpp Author: Vader Created on 2017. június 11., 16:28

#pragma once

// ext
#include <cereal/cereal.hpp>
// pro
#include <libv/serialization/enable.hpp>


#define LIBV_NVP(NAME) CEREAL_NVP(NAME)
#define LIBV_NVP_NAMED(NAME, VAR) CEREAL_NVP_(NAME, VAR)

#define LIBV_SERIALIZATION_EXTENSION_NAMESPACE cereal
