// Project: libv.ui, File: src/libv/ui/property/anchor.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/vec.hpp>
// std
#include <cstdint>
#include <iosfwd>
#include <span>
#include <string_view>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

/// Enum generator version: v3
struct Anchor {
	using value_type = uint16_t;

public:
	static const Anchor& top_left;
	static const Anchor& top_center;
	static const Anchor& top_right;
	static const Anchor& center_left;
	static const Anchor& center_center;
	static const Anchor& center_right;
	static const Anchor& bottom_left;
	static const Anchor& bottom_center;
	static const Anchor& bottom_right;

private:
    value_type value{0}; // top_left

public:
    constexpr inline Anchor() noexcept = default;
    constexpr inline Anchor(const Anchor& other) noexcept = default;
    constexpr inline Anchor(Anchor&& other) noexcept = default;
    constexpr inline Anchor& operator=(const Anchor& other) & noexcept = default;
    constexpr inline Anchor& operator=(Anchor&& other) & noexcept = default;
    inline ~Anchor() noexcept = default;

    explicit constexpr inline Anchor(value_type value) : value{value} { }

	constexpr inline operator value_type() const noexcept {
        return value;
    }

	[[nodiscard]] constexpr inline value_type to_value() const noexcept {{
        return value;
    }}

public:
	[[nodiscard]] friend constexpr inline bool operator==(const Anchor& lhs, const Anchor& rhs) noexcept {
		return lhs.value == rhs.value;
	}
	[[nodiscard]] friend constexpr inline bool operator!=(const Anchor& lhs, const Anchor& rhs) noexcept {
		return lhs.value != rhs.value;
	}
	[[nodiscard]] friend constexpr inline bool operator<(const Anchor& lhs, const Anchor& rhs) noexcept {
		return lhs.value < rhs.value;
	}
	[[nodiscard]] friend constexpr inline bool operator<=(const Anchor& lhs, const Anchor& rhs) noexcept {
		return lhs.value <= rhs.value;
	}
	[[nodiscard]] friend constexpr inline bool operator>(const Anchor& lhs, const Anchor& rhs) noexcept {
		return lhs.value > rhs.value;
	}
	[[nodiscard]] friend constexpr inline bool operator>=(const Anchor& lhs, const Anchor& rhs) noexcept {
		return lhs.value >= rhs.value;
	}

public:
	[[nodiscard]] std::string_view to_string() const noexcept;
	friend std::ostream& operator<<(std::ostream& os, const Anchor& var);

public:
	[[nodiscard]] static std::span<const Anchor> list() noexcept;

// TODO P5: Parsers for the property enums
//	[[nodiscard]] static std::optional<Anchor> parse(std::string_view text) noexcept;
//	[[nodiscard]] static Anchor parse_or(std::string_view text, const Anchor fallback) noexcept;
//	[[nodiscard]] static Anchor parse_throw(std::string_view text) noexcept;

public:
	[[nodiscard]] libv::vec3f to_info() const noexcept;
};

// <editor-fold defaultstate="collapsed" desc="Anchor constant declarations ...">

namespace detail {
static constexpr Anchor storage_Anchor[] = {
	Anchor{0}, // top_left
	Anchor{1}, // top_center
	Anchor{2}, // top_right
	Anchor{3}, // center_left
	Anchor{4}, // center_center
	Anchor{5}, // center_right
	Anchor{6}, // bottom_left
	Anchor{7}, // bottom_center
	Anchor{8}, // bottom_right
};
} // namespace detail

inline const Anchor& Anchor::top_left     {detail::storage_Anchor[0]};
inline const Anchor& Anchor::top_center   {detail::storage_Anchor[1]};
inline const Anchor& Anchor::top_right    {detail::storage_Anchor[2]};
inline const Anchor& Anchor::center_left  {detail::storage_Anchor[3]};
inline const Anchor& Anchor::center_center{detail::storage_Anchor[4]};
inline const Anchor& Anchor::center_right {detail::storage_Anchor[5]};
inline const Anchor& Anchor::bottom_left  {detail::storage_Anchor[6]};
inline const Anchor& Anchor::bottom_center{detail::storage_Anchor[7]};
inline const Anchor& Anchor::bottom_right {detail::storage_Anchor[8]};

// </editor-fold>

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
