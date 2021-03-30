// Project: libv, File: app/enum/example.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <span>
#include <cstdint>
#include <string_view>
// pro
#include <libv/math/vec.hpp>
#include <ostream>


namespace libv::ns {
namespace color {

// -------------------------------------------------------------------------------------------------

enum class color : int32_t {
	red = 0,
	green = 1,
	blue = 2,
};

struct color_type {
private:
	color enum_value_;

public:
	/* implicit */ constexpr inline color_type(color value) noexcept :
			enum_value_(value) {
	}

	[[nodiscard]] constexpr inline operator color() const noexcept {
		return enum_value_;
	}

	[[nodiscard]] constexpr inline color enum_value() const noexcept {
		return enum_value_;
	}

	[[nodiscard]] constexpr inline int32_t underlying() const noexcept {
		return static_cast<int32_t>(enum_value_);
	}

	[[nodiscard]] constexpr inline color value() const noexcept {
		return enum_value();
	}

private:
	static constexpr std::string_view table_to_string[] = {
			"red", // red
			"green", // green
			"blue", // blue
	};

public:
	[[nodiscard]] constexpr inline std::string_view to_string() const noexcept {
		assert(underlying() >= 0 && underlying() < 3);
		return table_to_string[underlying()];
	}

	friend std::ostream& operator<<(std::ostream& os, const color_type var) {
		return os << var.to_string();
	}

private:
	static constexpr libv::vec4f table_rgb[] = {
			libv::vec4f{1, 0, 0, 1}, // red
			libv::vec4f{0, 1, 0, 1}, // green
			libv::vec4f{0, 0, 1, 1}, // blue
	};

public:
	[[nodiscard]] constexpr inline libv::vec4f rgb() const noexcept {
		assert(underlying() >= 0 && underlying() < 3);
		return table_rgb[underlying()];
	}
};

class color_state {
	static constexpr color table_enum_values[] = {
			color::red,
			color::green,
			color::blue,
	};

public:
	static constexpr size_t size = 3;

	[[nodiscard]] static std::span<const color> values() noexcept {
		return table_enum_values;
	}
};

[[nodiscard]] constexpr inline std::string_view to_string(color enum_value) noexcept {
	return color_type(enum_value).to_string();
}

[[nodiscard]] constexpr inline color_type type(color enum_value) noexcept {
	return color_type(enum_value);
}

static constexpr color_state color_enum;

// -------------------------------------------------------------------------------------------------

} // namespace color
} // namespace libv::ns
