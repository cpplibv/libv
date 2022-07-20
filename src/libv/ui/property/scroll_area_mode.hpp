//
// Generated source code for enum: ScrollAreaMode
// Generator version: enum v2.5.1
// Input file: src/libv/ui/property/scroll_area_mode.hpp.in.lua

#pragma once

// std
#include <cassert>
#include <cstdint>
#include <ostream>
#include <span>
#include <string_view>


namespace libv::ui {

// -------------------------------------------------------------------------------------------------

enum class ScrollAreaMode : int32_t {
	both = 0,
	horizontal = 1,
	vertical = 2,
};

// --- ScrollAreaMode_type ---

struct ScrollAreaMode_type {
public:
	using enum_type = ScrollAreaMode;
	using underlying_type = int32_t;

private:
	enum_type enum_value_;

public:
	explicit(false) constexpr inline ScrollAreaMode_type(enum_type value) noexcept :
		enum_value_(value) {
		assert(underlying() >= 0 && underlying() < 3);
	}

	explicit(false) constexpr inline operator enum_type() const noexcept {
		return enum_value_;
	}

	[[nodiscard]] constexpr inline enum_type enum_value() const noexcept {
		return enum_value_;
	}

	[[nodiscard]] constexpr inline underlying_type underlying() const noexcept {
		return static_cast<underlying_type>(enum_value_);
	}

	[[nodiscard]] constexpr inline underlying_type operator+() const noexcept {
		return static_cast<underlying_type>(enum_value_);
	}

	[[nodiscard]] constexpr inline enum_type value() const noexcept {
		return enum_value();
	}

public:
	[[nodiscard]] constexpr inline enum_type next() noexcept {
		const underlying_type u = static_cast<underlying_type>(enum_value_);
		return enum_type{u == 2 ? 0 : u + 1};
	}
	[[nodiscard]] constexpr inline enum_type prev() noexcept {
		const underlying_type u = static_cast<underlying_type>(enum_value_);
		return enum_type{u == 0 ? 2 : u - 1};
	}

private:
	static constexpr std::string_view table_to_string[] = {
			"both",       // both
			"horizontal", // horizontal
			"vertical",   // vertical
	};

public:
	[[nodiscard]] constexpr inline std::string_view to_string() const noexcept {
		return table_to_string[static_cast<underlying_type>(enum_value_)];
	}
};

// --- ScrollAreaMode_state ---

class ScrollAreaMode_state {
public:
	using enum_type = ScrollAreaMode;
	using underlying_type = int32_t;

private:
	static constexpr enum_type table_enum_values[] = {
			enum_type::both,
			enum_type::horizontal,
			enum_type::vertical,
	};

public:
	static constexpr std::size_t size = 3;

	[[nodiscard]] static std::span<const enum_type> values() noexcept {
		return table_enum_values;
	}
};

// --- Global ADL functions ---

[[nodiscard]] constexpr inline std::string_view to_string(ScrollAreaMode enum_value) noexcept {
	return ScrollAreaMode_type(enum_value).to_string();
}
inline std::ostream& operator<<(std::ostream& os, const ScrollAreaMode var) {
	return os << ScrollAreaMode_type(var).to_string();
}

[[nodiscard]] constexpr inline int32_t underlying(ScrollAreaMode enum_value) noexcept {
	return static_cast<int32_t>(enum_value);
}
[[nodiscard]] constexpr inline int32_t operator+(ScrollAreaMode enum_value) noexcept {
	return static_cast<int32_t>(enum_value);
}

[[nodiscard]] constexpr inline ScrollAreaMode_type type(ScrollAreaMode enum_value) noexcept {
	return ScrollAreaMode_type(enum_value);
}
[[nodiscard]] constexpr inline ScrollAreaMode_type info(ScrollAreaMode enum_value) noexcept {
	return ScrollAreaMode_type(enum_value);
}

// --- Global state variables ---

static constexpr ScrollAreaMode_state ScrollAreaMode_enum;

// -------------------------------------------------------------------------------------------------

} // namespace libv::ui
