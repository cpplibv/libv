// Project: libv.thread, File: src/libv/thread/name.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <string>
// libv
#include <libv/thread/number.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

namespace detail {

inline thread_local std::string thread_name;

} // namespace detail

inline const std::string& thread_name(std::string name) {
	return detail::thread_name = std::move(name);
}

inline const std::string& thread_name() {
	return detail::thread_name;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv