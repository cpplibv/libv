//
// Generated source code for enum: Anchor
// Generator version: enum v2.5.2
// Input file: src/libv/ui/property/anchor.hpp.in.lua

#pragma once

// std
#include <cassert>
#include <cstdint>
#include <ostream>
#include <span>
#include <string_view>
//
#include <libv/math/vec.hpp>


namespace libv::ui {

// -------------------------------------------------------------------------------------------------

enum class Anchor : int32_t {
	top_left = 0,
	top_center = 1,
	top_right = 2,
	center_left = 3,
	center_center = 4,
	center_right = 5,
	bottom_left = 6,
	bottom_center = 7,
	bottom_right = 8,
};

// --- Anchor_type ---

struct Anchor_type {
public:
	using enum_type = Anchor;
	using underlying_type = int32_t;

private:
	enum_type enum_value_;

public:
	explicit(false) constexpr inline Anchor_type(enum_type value) noexcept :
		enum_value_(value) {
		assert(underlying() >= 0 && underlying() < 9);
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
		return static_cast<enum_type>(u == 8 ? 0 : u + 1);
	}
	[[nodiscard]] constexpr inline enum_type prev() noexcept {
		const underlying_type u = static_cast<underlying_type>(enum_value_);
		return static_cast<enum_type>(u == 0 ? 8 : u - 1);
	}

private:
	static constexpr std::string_view table_to_string[] = {
			"top-left",      // top_left
			"top-center",    // top_center
			"top-right",     // top_right
			"center-left",   // center_left
			"center-center", // center_center
			"center-right",  // center_right
			"bottom-left",   // bottom_left
			"bottom-center", // bottom_center
			"bottom-right",  // bottom_right
	};

public:
	[[nodiscard]] constexpr inline std::string_view to_string() const noexcept {
		return table_to_string[static_cast<underlying_type>(enum_value_)];
	}

private:
	static constexpr libv::vec3f table_rate[] = {
			libv::vec3f{0.0f, 1.0f, 0.0f}, // top_left
			libv::vec3f{0.5f, 1.0f, 0.0f}, // top_center
			libv::vec3f{1.0f, 1.0f, 0.0f}, // top_right
			libv::vec3f{0.0f, 0.5f, 0.0f}, // center_left
			libv::vec3f{0.5f, 0.5f, 0.0f}, // center_center
			libv::vec3f{1.0f, 0.5f, 0.0f}, // center_right
			libv::vec3f{0.0f, 0.0f, 0.0f}, // bottom_left
			libv::vec3f{0.5f, 0.0f, 0.0f}, // bottom_center
			libv::vec3f{1.0f, 0.0f, 0.0f}, // bottom_right
	};

public:
	[[nodiscard]] constexpr inline libv::vec3f rate() const noexcept {
		return table_rate[static_cast<underlying_type>(enum_value_)];
	}
};

// --- Anchor_state ---

class Anchor_state {
public:
	using enum_type = Anchor;
	using underlying_type = int32_t;

private:
	static constexpr enum_type table_enum_values[] = {
			enum_type::top_left,
			enum_type::top_center,
			enum_type::top_right,
			enum_type::center_left,
			enum_type::center_center,
			enum_type::center_right,
			enum_type::bottom_left,
			enum_type::bottom_center,
			enum_type::bottom_right,
	};

public:
	static constexpr std::size_t size = 9;

	[[nodiscard]] static std::span<const enum_type> values() noexcept {
		return table_enum_values;
	}
};

// --- Global ADL functions ---

[[nodiscard]] constexpr inline std::string_view to_string(Anchor enum_value) noexcept {
	return Anchor_type(enum_value).to_string();
}
inline std::ostream& operator<<(std::ostream& os, const Anchor var) {
	return os << Anchor_type(var).to_string();
}

[[nodiscard]] constexpr inline int32_t underlying(Anchor enum_value) noexcept {
	return static_cast<int32_t>(enum_value);
}
[[nodiscard]] constexpr inline int32_t operator+(Anchor enum_value) noexcept {
	return static_cast<int32_t>(enum_value);
}

[[nodiscard]] constexpr inline Anchor_type type(Anchor enum_value) noexcept {
	return Anchor_type(enum_value);
}
[[nodiscard]] constexpr inline Anchor_type info(Anchor enum_value) noexcept {
	return Anchor_type(enum_value);
}

// --- Global state variables ---

static constexpr Anchor_state Anchor_enum;

// -------------------------------------------------------------------------------------------------

} // namespace libv::ui
