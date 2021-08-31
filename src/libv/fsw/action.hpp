// Project: libv.fsw, File: src/libv/fsw/action.hpp, Author: Császár Mátyás [Vader]

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
//	prerun = 5,
};

std::ostream& operator<<(std::ostream& os, const Action& event);

// -------------------------------------------------------------------------------------------------

} // namespace fsw
} // namespace libv
