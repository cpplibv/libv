// File: file_watcher.hpp Author: Vader Created on 2017. július 8., 23:19

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
