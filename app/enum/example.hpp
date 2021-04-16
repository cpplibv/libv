// Project: libv, File: app/enum/example.hpp, Author: Császár Mátyás [Vader]
// Generator: enum v14

#pragma once

// std
#include <cassert>
#include <iosfwd>
#include <ostream>
#include <span>
#include <string_view>
//
#include <libv/math/vec.hpp>


namespace libv::ns {

// -------------------------------------------------------------------------------------------------

enum class color : int32_t {
	red = 0,
	green = 1,
	blue = 2,
};

// --- color_type ---

struct color_type {
public:
	using enum_type = color;
	using underlying_type = int32_t;

private:
	enum_type enum_value_;

public:
	/* implicit */ constexpr inline color_type(enum_type value) noexcept :
			enum_value_(value) {
	}

	/* implicit */ [[nodiscard]] constexpr inline operator enum_type() const noexcept {
		return enum_value_;
	}

	[[nodiscard]] constexpr inline enum_type enum_value() const noexcept {
		return enum_value_;
	}

	[[nodiscard]] constexpr inline underlying_type underlying() const noexcept {
		return static_cast<underlying_type>(enum_value_);
	}

	[[nodiscard]] constexpr inline enum_type value() const noexcept {
		return enum_value();
	}

private:
	static constexpr std::string_view table_to_string[] = {
			"Red",   // red
			"Green", // green
			"Blue",  // blue
	};

public:
	[[nodiscard]] constexpr inline std::string_view to_string() const noexcept {
		assert(underlying() >= 0 && underlying() < 3);
		return table_to_string[underlying()];
	}

private:
	static constexpr libv::vec4f table_rgba[] = {
			libv::vec4f{1, 0, 0, 1}, // red
			libv::vec4f{0, 1, 0, 1}, // green
			libv::vec4f{0, 0, 1, 1}, // blue
	};

public:
	[[nodiscard]] constexpr inline libv::vec4f rgba() const noexcept {
		assert(underlying() >= 0 && underlying() < 3);
		return table_rgba[underlying()];
	}
};

// --- color_state ---

class color_state {
public:
	using enum_type = color;
	using underlying_type = int32_t;

private:
	static constexpr enum_type table_enum_values[] = {
			enum_type::red,
			enum_type::green,
			enum_type::blue,
	};

public:
	static constexpr size_t size = 3;

	[[nodiscard]] static std::span<const enum_type> values() noexcept {
		return table_enum_values;
	}
};

// --- Global ADL functions ---

[[nodiscard]] constexpr inline std::string_view to_string(color enum_value) noexcept {
	return color_type(enum_value).to_string();
}

std::ostream& operator<<(std::ostream& os, const color var) {
	return os << color_type(var).to_string();
}

[[nodiscard]] constexpr inline int32_t underlying(color enum_value) noexcept {
	return static_cast<int32_t>(enum_value);
}

[[nodiscard]] constexpr inline color_type type(color enum_value) noexcept {
	return color_type(enum_value);
}

// --- Global state variables ---

static constexpr color_state color_enum;

// -------------------------------------------------------------------------------------------------

} // namespace libv::ns
