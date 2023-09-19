// Project: libv.fsw, File: src/libv/fsw/token.hpp

#pragma once

#include <libv/fsw/fwd.hpp>


namespace libv {
namespace fsw {

// -------------------------------------------------------------------------------------------------

struct Token {
	// TODO P5: hide or use a better token | not the worst
	void* id = nullptr;

	constexpr inline Token() noexcept = default;
	explicit constexpr inline Token(void* id) noexcept : id(id) { }

	constexpr inline Token(const Token&) noexcept = default;
	constexpr inline Token& operator=(const Token&) & noexcept = default;
	constexpr inline Token(Token&& other) noexcept :
		id(other.id) {
		other.id = nullptr;
	};
	constexpr inline Token& operator=(Token&& other) & noexcept {
		id = other.id;
		other.id = nullptr;
		return *this;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace fsw
} // namespace libv
