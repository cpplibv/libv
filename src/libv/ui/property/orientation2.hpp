//
// Generated source code for enum: Orientation2
// Generator version: enum v2.4.2
// Input file: src/libv/ui/property/orientation2.hpp.in.lua

#pragma once

// std
#include <cassert>
#include <cstdint>
#include <ostream>
#include <span>
#include <string_view>


namespace libv::ui {

// -------------------------------------------------------------------------------------------------

enum class Orientation2 : int32_t {
	down_left = 0,
	down_right = 1,
	left_down = 2,
	left_up = 3,
	right_down = 4,
	right_up = 5,
	up_left = 6,
	up_right = 7,
};

// --- Orientation2_type ---

struct Orientation2_type {
public:
	using enum_type = Orientation2;
	using underlying_type = int32_t;

private:
	enum_type enum_value_;

public:
	explicit(false) constexpr inline Orientation2_type(enum_type value) noexcept :
		enum_value_(value) {
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

private:
	static constexpr std::string_view table_to_string[] = {
			"down-left",  // down_left
			"down-right", // down_right
			"left-down",  // left_down
			"left-up",    // left_up
			"right-down", // right_down
			"right-up",   // right_up
			"up-left",    // up_left
			"up-right",   // up_right
	};

public:
	[[nodiscard]] constexpr inline std::string_view to_string() const noexcept {
		assert(underlying() >= 0 && underlying() < 8);
		return table_to_string[underlying()];
	}
};

// --- Orientation2_state ---

class Orientation2_state {
public:
	using enum_type = Orientation2;
	using underlying_type = int32_t;

private:
	static constexpr enum_type table_enum_values[] = {
			enum_type::down_left,
			enum_type::down_right,
			enum_type::left_down,
			enum_type::left_up,
			enum_type::right_down,
			enum_type::right_up,
			enum_type::up_left,
			enum_type::up_right,
	};

public:
	static constexpr size_t size = 8;

	[[nodiscard]] static std::span<const enum_type> values() noexcept {
		return table_enum_values;
	}
};

// --- Global ADL functions ---

[[nodiscard]] constexpr inline std::string_view to_string(Orientation2 enum_value) noexcept {
	return Orientation2_type(enum_value).to_string();
}
inline std::ostream& operator<<(std::ostream& os, const Orientation2 var) {
	return os << Orientation2_type(var).to_string();
}

[[nodiscard]] constexpr inline int32_t underlying(Orientation2 enum_value) noexcept {
	return static_cast<int32_t>(enum_value);
}
[[nodiscard]] constexpr inline int32_t operator+(Orientation2 enum_value) noexcept {
	return static_cast<int32_t>(enum_value);
}

[[nodiscard]] constexpr inline Orientation2_type type(Orientation2 enum_value) noexcept {
	return Orientation2_type(enum_value);
}
[[nodiscard]] constexpr inline Orientation2_type info(Orientation2 enum_value) noexcept {
	return Orientation2_type(enum_value);
}

// --- Global state variables ---

static constexpr Orientation2_state Orientation2_enum;

// -------------------------------------------------------------------------------------------------

} // namespace libv::ui
