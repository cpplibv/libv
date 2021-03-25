// First line

#pragma once

// std
#include <span>
#include <cstdint>
#include <string_view>
// pro
#include <libv/math/vec.hpp>


namespace ms::ns {

// -------------------------------------------------------------------------------------------------

enum class color : int32_t {
	red = 0,
	green = 1,
	blue = 2,
};

struct color_type {
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

public:
	[[nodiscard]] constexpr inline libv::vec3f rgb() const noexcept {
		static constexpr libv::vec3f table[] = {
				libv::vec3f(1, 0, 0),
				libv::vec3f(0, 1, 0),
				libv::vec3f(0, 0, 1),
		};

		assert(underlying() >= 0 && underlying() < 3);
		return table[underlying()];
	}

	[[nodiscard]] constexpr inline libv::vec4f rgba() const noexcept {
		static constexpr libv::vec4f table[] = {
				libv::vec4f(1, 0, 0, 1),
				libv::vec4f(0, 1, 0, 1),
				libv::vec4f(0, 0, 1, 1),
		};

		assert(underlying() >= 0 && underlying() < 3);
		return table[underlying()];
	}

public:
	[[nodiscard]] constexpr inline std::string_view to_string() const noexcept {
		static constexpr std::string_view table[] = {
				"red",
				"green",
				"blue",
		};

		assert(underlying() >= 0 && underlying() < 3);
		return table[underlying()];
	}

	// ostream
	// parse
	// operator<=>
	// hash
};

[[nodiscard]] constexpr inline color_type type(color enum_value) noexcept {
	return color_type(enum_value);
}

class color_state {
	static constexpr color table[] = {
			color::red,
			color::green,
			color::blue,
	};

public:
	static constexpr size_t size = 3;

	// Range interfaces over table
};

static constexpr color_state color_enum;

// -------------------------------------------------------------------------------------------------

} // namespace ms::ns
