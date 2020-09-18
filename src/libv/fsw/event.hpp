// Project: libv.fsw, File: src/libv/fsw/event.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <filesystem>
#include <iosfwd>
// pro
#include <libv/fsw/action.hpp>


namespace libv {
namespace fsw {

// -------------------------------------------------------------------------------------------------

struct Event {
	Action action;
	std::filesystem::path path;
	std::filesystem::path old_path;

	friend std::ostream& operator<<(std::ostream& os, const Event& event);
};

// -------------------------------------------------------------------------------------------------

} // namespace fsw
} // namespace libv
