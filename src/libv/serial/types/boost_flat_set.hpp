// Project: libv.serial, File: src/libv/serial/types/boost_flat_set.hpp

#pragma once

// ext
#include <boost/container/flat_set.hpp>
#include <vide/types/set.hpp>
// pro
#include <libv/serial/serial.hpp>


namespace LIBV_SERIAL_EXTENSION_NAMESPACE {

// -------------------------------------------------------------------------------------------------

template <class Archive, class K, class C, class A>
inline void LIBV_FUNCTION_NAME_SAVE(Archive& ar, const boost::container::flat_set<K, C, A>& set) {
	vide::set_detail::save(ar, set);
}

template <class Archive, class K, class C, class A>
inline void LIBV_FUNCTION_NAME_LOAD(Archive& ar, boost::container::flat_set<K, C, A>& set) {
	vide::set_detail::load(ar, set);
}

// -------------------------------------------------------------------------------------------------

} // namespace LIBV_SERIAL_EXTENSION_NAMESPACE
