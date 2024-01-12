// Project: libv.fsw, File: src/libv/fsw/token.hpp

#pragma once

#include <libv/fsw/fwd.hpp>

#include <utility>


namespace libv {
namespace fsw {

// -------------------------------------------------------------------------------------------------

struct Token {
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

struct SafeToken {
private:
	Token token;
	Watcher* watcher = nullptr;

public:
	constexpr inline SafeToken(Token token, Watcher& watcher) : token(std::move(token)), watcher(&watcher) { }
	constexpr inline SafeToken(const SafeToken&) = delete;
	constexpr inline SafeToken& operator=(const SafeToken&) & = delete;
	constexpr inline SafeToken(SafeToken&&) noexcept = default;
	constexpr inline SafeToken& operator=(SafeToken&&) & noexcept = default;
	~SafeToken();
};

// -------------------------------------------------------------------------------------------------

} // namespace fsw
} // namespace libv
