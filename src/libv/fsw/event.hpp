// Project: libv.fsw, File: src/libv/fsw/event.hpp

#pragma once

#include <libv/fsw/fwd.hpp>
#include <libv/fsw/action.hpp>

#include <filesystem>
#include <iosfwd>


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
