// File: nvp.hpp, Created on 2015. február 14. 23:44, Author: Vader

#pragma once

// ext
#include <boost/serialization/nvp.hpp>
// vl
#include <vl/vm3/config.hpp>

#ifdef VM3_NO_XML_ARCHIVE
#    define VL_NVP(NAME) NAME
#    define VL_NVP_NAMED(NAME, VAR) VAR
#    define VL_NVP_BASE_OBJECT(BASE) ::boost::serialization::base_object<BASE>(*this)
#else
#    define VL_NVP(NAME) ::BOOST_SERIALIZATION_NVP(NAME)
#    define VL_NVP_NAMED(NAME, VAR) ::boost::serialization::make_nvp(NAME, VAR)
#    define VL_NVP_BASE_OBJECT(BASE) ::BOOST_SERIALIZATION_BASE_OBJECT_NVP(BASE)
#endif