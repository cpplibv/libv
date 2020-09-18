// File: file_watcher.hpp Author: Vader Created on 2017. július 8., 23:19

#pragma once


namespace libv {
namespace fsw {

// -------------------------------------------------------------------------------------------------

struct WatchToken {
	// TODO P5: hide or use a better token
	void* id = nullptr;

	constexpr inline WatchToken() noexcept = default;
	constexpr inline WatchToken(void* id) noexcept : id(id) { }
};

// -------------------------------------------------------------------------------------------------

} // namespace fsw
} // namespace libv
