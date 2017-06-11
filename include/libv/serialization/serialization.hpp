// File: serialization.hpp Author: Vader Created on 2017. június 11., 16:28

#pragma once

#ifndef LIBV_USE_SERIALIZATION_CEREAL // ===========================================================

// ext
#    include <boost/serialization/nvp.hpp>
#    include <boost/serialization/serialization.hpp>


#    define LIBV_NVP(NAME) ::BOOST_SERIALIZATION_NVP(NAME)
#    define LIBV_NVP_NAMED(NAME, VAR) ::boost::serialization::make_nvp(NAME, VAR)

#    define LIBV_SERIALIZATION_SAVE_FUNCTION_NAME save
#    define LIBV_SERIALIZATION_LOAD_FUNCTION_NAME load
#    define LIBV_SERIALIZATION_EXTENSION_NAMESPACE boost::serialization

#else // ===========================================================================================

// ext
#    include <cereal/cereal.hpp>


#    define LIBV_NVP(NAME) CEREAL_NVP(NAME)
#    define LIBV_NVP_NAMED(NAME, VAR) CEREAL_NVP_(NAME, VAR)

#    define LIBV_SERIALIZATION_SAVE_FUNCTION_NAME save
#    define LIBV_SERIALIZATION_LOAD_FUNCTION_NAME load
#    define LIBV_SERIALIZATION_EXTENSION_NAMESPACE cereal

#endif // ==========================================================================================

