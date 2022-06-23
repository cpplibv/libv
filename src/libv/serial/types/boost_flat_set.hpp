// Project: libv.serial, File: src/libv/serial/types/boost_flat_set.hpp

#pragma once

// ext
#include <boost/container/flat_set.hpp>
#include <vide/types/set.hpp>
// pro
#include <libv/serial/serial.hpp>


namespace LIBV_SERIAL_EXTENSION_NAMESPACE {

//! Saving for boost::container::flat_set
template <class Archive, class K, class C, class A> inline
void VIDE_SAVE_FUNCTION_NAME(Archive& ar, const boost::container::flat_set<K, C, A>& set) {
	set_detail::save(ar, set);
}

//! Loading for boost::container::flat_set
template <class Archive, class K, class C, class A> inline
void VIDE_LOAD_FUNCTION_NAME(Archive& ar, boost::container::flat_set<K, C, A>& set) {
	set_detail::load(ar, set);
}

} // namespace LIBV_SERIAL_EXTENSION_NAMESPACE
