// Project: libv.fsw, File: src/libv/fsw/token.hpp

#pragma once


namespace libv {
namespace fsw {

// -------------------------------------------------------------------------------------------------

struct Token {
	// TODO P5: hide or use a better token | not the worst
	void* id = nullptr;

	constexpr inline Token() noexcept = default;
	explicit constexpr inline Token(void* id) noexcept : id(id) { }
};

// -------------------------------------------------------------------------------------------------

} // namespace fsw
} // namespace libv
