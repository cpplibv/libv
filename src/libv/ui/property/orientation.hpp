//
// Generated source code for enum: Orientation
// Generator version: enum v2.5.2
// Input file: src/libv/ui/property/orientation.hpp.in.lua

#pragma once

// std
#include <cassert>
#include <cstdint>
#include <ostream>
#include <span>
#include <string_view>


namespace libv::ui {

// -------------------------------------------------------------------------------------------------

enum class Orientation : int32_t {
	up = 0,
	right = 1,
	left = 2,
	down = 3,
};

// --- Orientation_type ---

struct Orientation_type {
public:
	using enum_type = Orientation;
	using underlying_type = int32_t;

private:
	enum_type enum_value_;

public:
	explicit(false) constexpr inline Orientation_type(enum_type value) noexcept :
		enum_value_(value) {
		assert(underlying() >= 0 && underlying() < 4);
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
			"up",    // up
			"right", // right
			"left",  // left
			"down",  // down
	};

public:
	[[nodiscard]] constexpr inline std::string_view to_string() const noexcept {
		return table_to_string[static_cast<underlying_type>(enum_value_)];
	}
};

// --- Orientation_state ---

class Orientation_state {
public:
	using enum_type = Orientation;
	using underlying_type = int32_t;

private:
	static constexpr enum_type table_enum_values[] = {
			enum_type::up,
			enum_type::right,
			enum_type::left,
			enum_type::down,
	};

public:
	static constexpr std::size_t size = 4;

	[[nodiscard]] static std::span<const enum_type> values() noexcept {
		return table_enum_values;
	}
};

// --- Global ADL functions ---

[[nodiscard]] constexpr inline std::string_view to_string(Orientation enum_value) noexcept {
	return Orientation_type(enum_value).to_string();
}
inline std::ostream& operator<<(std::ostream& os, const Orientation var) {
	return os << Orientation_type(var).to_string();
}

[[nodiscard]] constexpr inline int32_t underlying(Orientation enum_value) noexcept {
	return static_cast<int32_t>(enum_value);
}
[[nodiscard]] constexpr inline int32_t operator+(Orientation enum_value) noexcept {
	return static_cast<int32_t>(enum_value);
}

[[nodiscard]] constexpr inline Orientation_type type(Orientation enum_value) noexcept {
	return Orientation_type(enum_value);
}
[[nodiscard]] constexpr inline Orientation_type info(Orientation enum_value) noexcept {
	return Orientation_type(enum_value);
}

// --- Global state variables ---

static constexpr Orientation_state Orientation_enum;

// -------------------------------------------------------------------------------------------------

} // namespace libv::ui
