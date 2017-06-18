// File: serialization.hpp Author: Vader Created on 2017. június 11., 16:28

#pragma once

#ifndef LIBV_USE_SERIALIZATION_CEREAL // ===========================================================

// ext
#    include <boost/serialization/nvp.hpp>
#    include <boost/serialization/serialization.hpp>


#    define LIBV_NVP(NAME) ::BOOST_SERIALIZATION_NVP(NAME)
#    define LIBV_NVP_NAMED(NAME, VAR) ::boost::serialization::make_nvp(NAME, VAR)

#    define LIBV_SERIALIZATION_EXTENSION_NAMESPACE boost::serialization

#else // ===========================================================================================

// ext
#    include <cereal/cereal.hpp>


#    define LIBV_NVP(NAME) CEREAL_NVP(NAME)
#    define LIBV_NVP_NAMED(NAME, VAR) CEREAL_NVP_(NAME, VAR)

#    define LIBV_SERIALIZATION_EXTENSION_NAMESPACE cereal

#endif // ==========================================================================================


#define LIBV_SERIALIAZTION_ENABLE_MEMBER using __libv_serialiaztion_enable_member = void;
#define LIBV_SERIALIAZTION_ENABLE_MEMBERWISE using __libv_serialiaztion_enable_memberwise = void;
