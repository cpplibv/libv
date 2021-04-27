// Project: libv.fsw, File: src/libv/fsw/token.hpp, Author: Császár Mátyás [Vader]

#pragma once


namespace libv {
namespace fsw {

// -------------------------------------------------------------------------------------------------

struct WatchToken {
	// TODO P5: hide or use a better token
	void* id = nullptr;

	constexpr inline WatchToken() noexcept = default;
	explicit constexpr inline WatchToken(void* id) noexcept : id(id) { }
};

// -------------------------------------------------------------------------------------------------

} // namespace fsw
} // namespace libv
