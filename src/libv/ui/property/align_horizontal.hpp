//
// Generated source code for enum: AlignHorizontal
// Generator version: enum v2.4.2
// Input file: src/libv/ui/property/align_horizontal.hpp.in.lua

#pragma once

// std
#include <cassert>
#include <cstdint>
#include <ostream>
#include <span>
#include <string_view>


namespace libv::ui {

// -------------------------------------------------------------------------------------------------

enum class AlignHorizontal : int32_t {
	right = 0,
	center = 1,
	left = 2,
	justify = 3,
	justify_all = 4,
};

// --- AlignHorizontal_type ---

struct AlignHorizontal_type {
public:
	using enum_type = AlignHorizontal;
	using underlying_type = int32_t;

private:
	enum_type enum_value_;

public:
	explicit(false) constexpr inline AlignHorizontal_type(enum_type value) noexcept :
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
			"right",       // right
			"center",      // center
			"left",        // left
			"justify",     // justify
			"justify-all", // justify_all
	};

public:
	[[nodiscard]] constexpr inline std::string_view to_string() const noexcept {
		assert(underlying() >= 0 && underlying() < 5);
		return table_to_string[underlying()];
	}

private:
	static constexpr bool table_justified[] = {
			false, // right
			false, // center
			false, // left
			true,  // justify
			true,  // justify_all
	};

public:
	[[nodiscard]] constexpr inline bool justified() const noexcept {
		assert(underlying() >= 0 && underlying() < 5);
		return table_justified[underlying()];
	}

private:
	static constexpr float table_rate[] = {
			1.0f, // right
			0.5f, // center
			0.0f, // left
			0.0f, // justify
			0.0f, // justify_all
	};

public:
	[[nodiscard]] constexpr inline float rate() const noexcept {
		assert(underlying() >= 0 && underlying() < 5);
		return table_rate[underlying()];
	}
};

// --- AlignHorizontal_state ---

class AlignHorizontal_state {
public:
	using enum_type = AlignHorizontal;
	using underlying_type = int32_t;

private:
	static constexpr enum_type table_enum_values[] = {
			enum_type::right,
			enum_type::center,
			enum_type::left,
			enum_type::justify,
			enum_type::justify_all,
	};

public:
	static constexpr size_t size = 5;

	[[nodiscard]] static std::span<const enum_type> values() noexcept {
		return table_enum_values;
	}
};

// --- Global ADL functions ---

[[nodiscard]] constexpr inline std::string_view to_string(AlignHorizontal enum_value) noexcept {
	return AlignHorizontal_type(enum_value).to_string();
}
inline std::ostream& operator<<(std::ostream& os, const AlignHorizontal var) {
	return os << AlignHorizontal_type(var).to_string();
}

[[nodiscard]] constexpr inline int32_t underlying(AlignHorizontal enum_value) noexcept {
	return static_cast<int32_t>(enum_value);
}
[[nodiscard]] constexpr inline int32_t operator+(AlignHorizontal enum_value) noexcept {
	return static_cast<int32_t>(enum_value);
}

[[nodiscard]] constexpr inline AlignHorizontal_type type(AlignHorizontal enum_value) noexcept {
	return AlignHorizontal_type(enum_value);
}
[[nodiscard]] constexpr inline AlignHorizontal_type info(AlignHorizontal enum_value) noexcept {
	return AlignHorizontal_type(enum_value);
}

// --- Global state variables ---

static constexpr AlignHorizontal_state AlignHorizontal_enum;

// -------------------------------------------------------------------------------------------------

} // namespace libv::ui
