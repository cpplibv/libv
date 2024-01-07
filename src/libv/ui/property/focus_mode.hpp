//
// Generated source code for enum: FocusMode
// Generator version: enum v2.5.2
// Input file: src/libv/ui/property/focus_mode.hpp.in.lua

#pragma once

// std
#include <cassert>
#include <cstdint>
#include <ostream>
#include <span>
#include <string_view>


namespace libv::ui {

// -------------------------------------------------------------------------------------------------

enum class FocusMode : int8_t {
	active = 0,
	passive = 1,
	inactive = 2,
};

// --- FocusMode_type ---

struct FocusMode_type {
public:
	using enum_type = FocusMode;
	using underlying_type = int8_t;

private:
	enum_type enum_value_;

public:
	explicit(false) constexpr inline FocusMode_type(enum_type value) noexcept :
		enum_value_(value) {
		assert(static_cast<underlying_type>(enum_value_) >= 0 && static_cast<underlying_type>(enum_value_) < 3);
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
		return static_cast<enum_type>(u == 2 ? 0 : u + 1);
	}
	[[nodiscard]] constexpr inline enum_type prev() noexcept {
		const underlying_type u = static_cast<underlying_type>(enum_value_);
		return static_cast<enum_type>(u == 0 ? 2 : u - 1);
	}

private:
	static constexpr std::string_view table_to_string[] = {
			"active",   // active
			"passive",  // passive
			"inactive", // inactive
	};

public:
	[[nodiscard]] constexpr inline std::string_view to_string() const noexcept {
		return table_to_string[static_cast<underlying_type>(enum_value_)];
	}

private:
	static constexpr bool table_focusable[] = {
			true,  // active
			true,  // passive
			false, // inactive
	};
	static constexpr bool table_mouse[] = {
			true,  // active
			false, // passive
			false, // inactive
	};

public:
	[[nodiscard]] constexpr inline bool focusable() const noexcept {
		return table_focusable[static_cast<underlying_type>(enum_value_)];
	}
	[[nodiscard]] constexpr inline bool mouse() const noexcept {
		return table_mouse[static_cast<underlying_type>(enum_value_)];
	}
};

// --- FocusMode_state ---

class FocusMode_state {
public:
	using enum_type = FocusMode;
	using underlying_type = int8_t;

private:
	static constexpr enum_type table_enum_values[] = {
			enum_type::active,
			enum_type::passive,
			enum_type::inactive,
	};

public:
	static constexpr std::size_t size = 3;

	[[nodiscard]] static std::span<const enum_type> values() noexcept {
		return table_enum_values;
	}
};

// --- Global ADL functions ---

[[nodiscard]] constexpr inline std::string_view to_string(FocusMode enum_value) noexcept {
	return FocusMode_type(enum_value).to_string();
}
inline std::ostream& operator<<(std::ostream& os, const FocusMode var) {
	return os << FocusMode_type(var).to_string();
}

[[nodiscard]] constexpr inline int8_t underlying(FocusMode enum_value) noexcept {
	return static_cast<int8_t>(enum_value);
}
[[nodiscard]] constexpr inline int8_t operator+(FocusMode enum_value) noexcept {
	return static_cast<int8_t>(enum_value);
}

[[nodiscard]] constexpr inline FocusMode_type type(FocusMode enum_value) noexcept {
	return FocusMode_type(enum_value);
}
[[nodiscard]] constexpr inline FocusMode_type info(FocusMode enum_value) noexcept {
	return FocusMode_type(enum_value);
}

// --- Global state variables ---

static constexpr FocusMode_state FocusMode_enum;

// -------------------------------------------------------------------------------------------------

} // namespace libv::ui
