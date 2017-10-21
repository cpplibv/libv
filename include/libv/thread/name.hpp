// File: name.hpp - Created on 2017.10.21. 02:47 - Author: Vader

#pragma once

// std
#include <string>


namespace libv {

// -------------------------------------------------------------------------------------------------

namespace detail {

inline thread_local std::string thread_name = "unnamed";

} // namespace detail

inline const std::string& thread_name(std::string name) {
	return detail::thread_name = std::move(name);
}

inline const std::string& thread_name() {
	return detail::thread_name;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
