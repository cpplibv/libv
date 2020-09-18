// Project: libv.control, Created on 2020.03.16. 16:41, Author: Császár Mátyás [Vader]

#pragma once


namespace libv {
namespace ctrl {

// -------------------------------------------------------------------------------------------------

enum class search_mode {
	contains,
	ends_with,
	matches,
	starts_with,

	not_contains,
	not_ends_with,
	not_matches,
	not_starts_with,
};

// -------------------------------------------------------------------------------------------------

} //namespace ctrl
} //namespace libv
