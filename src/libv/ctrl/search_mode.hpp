// Project: libv.ctrl, File: src/libv/ctrl/search_mode.hpp

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
