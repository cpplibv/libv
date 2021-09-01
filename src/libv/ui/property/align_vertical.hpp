// Generated source code for enum: AlignVertical
// Generator version: enum v2.4.2
// Input file: src/libv/ui/property/align_vertical.hpp.in.lua

#pragma once

// std
#include <cassert>
#include <cstdint>
#include <ostream>
#include <span>
#include <string_view>


namespace libv::ui {

// -------------------------------------------------------------------------------------------------

enum class AlignVertical : int32_t {
	top = 0,
	center = 1,
	bottom = 2,
	justify = 3,
	justify_all = 4,
};

// --- AlignVertical_type ---

struct AlignVertical_type {
public:
	using enum_type = AlignVertical;
	using underlying_type = int32_t;

private:
	enum_type enum_value_;

public:
	explicit(false) constexpr inline AlignVertical_type(enum_type value) noexcept :
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
			"top",         // top
			"center",      // center
			"bottom",      // bottom
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
			false, // top
			false, // center
			false, // bottom
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
			1.0f, // top
			0.5f, // center
			0.0f, // bottom
			1.0f, // justify
			1.0f, // justify_all
	};

public:
	[[nodiscard]] constexpr inline float rate() const noexcept {
		assert(underlying() >= 0 && underlying() < 5);
		return table_rate[underlying()];
	}
};

// --- AlignVertical_state ---

class AlignVertical_state {
public:
	using enum_type = AlignVertical;
	using underlying_type = int32_t;

private:
	static constexpr enum_type table_enum_values[] = {
			enum_type::top,
			enum_type::center,
			enum_type::bottom,
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

[[nodiscard]] constexpr inline std::string_view to_string(AlignVertical enum_value) noexcept {
	return AlignVertical_type(enum_value).to_string();
}
inline std::ostream& operator<<(std::ostream& os, const AlignVertical var) {
	return os << AlignVertical_type(var).to_string();
}

[[nodiscard]] constexpr inline int32_t underlying(AlignVertical enum_value) noexcept {
	return static_cast<int32_t>(enum_value);
}
[[nodiscard]] constexpr inline int32_t operator+(AlignVertical enum_value) noexcept {
	return static_cast<int32_t>(enum_value);
}

[[nodiscard]] constexpr inline AlignVertical_type type(AlignVertical enum_value) noexcept {
	return AlignVertical_type(enum_value);
}
[[nodiscard]] constexpr inline AlignVertical_type info(AlignVertical enum_value) noexcept {
	return AlignVertical_type(enum_value);
}

// --- Global state variables ---

static constexpr AlignVertical_state AlignVertical_enum;

// -------------------------------------------------------------------------------------------------

} // namespace libv::ui
