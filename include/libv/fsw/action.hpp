// File: file_watcher.hpp Author: Vader Created on 2017. július 8., 23:19

#pragma once

// std
#include <iosfwd>


namespace libv {
namespace fsw {

// -------------------------------------------------------------------------------------------------

enum class Action {
	create = 1,
	remove = 2,
	modify = 3,
	rename = 4,
};

std::ostream& operator<<(std::ostream& os, const Action& event);

// -------------------------------------------------------------------------------------------------

} // namespace fsw
} // namespace libv
