//
// Generated source code for enum: ScrollMode
// Generator version: enum v2.5.2
// Input file: src/libv/ui/property/scroll_mode.hpp.in.lua

#pragma once

// std
#include <cassert>
#include <cstdint>
#include <ostream>
#include <span>
#include <string_view>


namespace libv::ui {

// -------------------------------------------------------------------------------------------------

enum class ScrollMode : int8_t {
	none = 0,
	both = 1,
	horizontal = 2,
	vertical = 3,
};

// --- ScrollMode_type ---

struct ScrollMode_type {
public:
	using enum_type = ScrollMode;
	using underlying_type = int8_t;

private:
	enum_type enum_value_;

public:
	explicit(false) constexpr inline ScrollMode_type(enum_type value) noexcept :
		enum_value_(value) {
		assert(static_cast<underlying_type>(enum_value_) >= 0 && static_cast<underlying_type>(enum_value_) < 4);
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
		return static_cast<enum_type>(u == 3 ? 0 : u + 1);
	}
	[[nodiscard]] constexpr inline enum_type prev() noexcept {
		const underlying_type u = static_cast<underlying_type>(enum_value_);
		return static_cast<enum_type>(u == 0 ? 3 : u - 1);
	}

private:
	static constexpr std::string_view table_to_string[] = {
			"none",       // none
			"both",       // both
			"horizontal", // horizontal
			"vertical",   // vertical
	};

public:
	[[nodiscard]] constexpr inline std::string_view to_string() const noexcept {
		return table_to_string[static_cast<underlying_type>(enum_value_)];
	}

private:
	static constexpr bool table_horizontal[] = {
			false, // none
			true,  // both
			true,  // horizontal
			false, // vertical
	};
	static constexpr bool table_vertical[] = {
			false, // none
			true,  // both
			false, // horizontal
			true,  // vertical
	};

public:
	[[nodiscard]] constexpr inline bool horizontal() const noexcept {
		return table_horizontal[static_cast<underlying_type>(enum_value_)];
	}
	[[nodiscard]] constexpr inline bool vertical() const noexcept {
		return table_vertical[static_cast<underlying_type>(enum_value_)];
	}
};

// --- ScrollMode_state ---

class ScrollMode_state {
public:
	using enum_type = ScrollMode;
	using underlying_type = int8_t;

private:
	static constexpr enum_type table_enum_values[] = {
			enum_type::none,
			enum_type::both,
			enum_type::horizontal,
			enum_type::vertical,
	};

public:
	static constexpr std::size_t size = 4;

	[[nodiscard]] static std::span<const enum_type> values() noexcept {
		return table_enum_values;
	}
};

// --- Global ADL functions ---

[[nodiscard]] constexpr inline std::string_view to_string(ScrollMode enum_value) noexcept {
	return ScrollMode_type(enum_value).to_string();
}
inline std::ostream& operator<<(std::ostream& os, const ScrollMode var) {
	return os << ScrollMode_type(var).to_string();
}

[[nodiscard]] constexpr inline int8_t underlying(ScrollMode enum_value) noexcept {
	return static_cast<int8_t>(enum_value);
}
[[nodiscard]] constexpr inline int8_t operator+(ScrollMode enum_value) noexcept {
	return static_cast<int8_t>(enum_value);
}

[[nodiscard]] constexpr inline ScrollMode_type type(ScrollMode enum_value) noexcept {
	return ScrollMode_type(enum_value);
}
[[nodiscard]] constexpr inline ScrollMode_type info(ScrollMode enum_value) noexcept {
	return ScrollMode_type(enum_value);
}

// --- Global state variables ---

static constexpr ScrollMode_state ScrollMode_enum;

// -------------------------------------------------------------------------------------------------

} // namespace libv::ui
