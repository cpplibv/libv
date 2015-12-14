// File: nvp.hpp, Created on 2015. február 14. 23:44, Author: Vader

#pragma once

// ext
#include <boost/serialization/nvp.hpp>
// libv
#include <libv/vm3/config.hpp>

#ifdef VM3_NO_XML_ARCHIVE
#    define LIBV_NVP(NAME) NAME
#    define LIBV_NVP_NAMED(NAME, VAR) VAR
#    define LIBV_NVP_BASE_OBJECT(BASE) ::boost::serialization::base_object<BASE>(*this)
#else
#    define LIBV_NVP(NAME) ::BOOST_SERIALIZATION_NVP(NAME)
#    define LIBV_NVP_NAMED(NAME, VAR) ::boost::serialization::make_nvp(NAME, VAR)
#    define LIBV_NVP_BASE_OBJECT(BASE) ::BOOST_SERIALIZATION_BASE_OBJECT_NVP(BASE)
#endif