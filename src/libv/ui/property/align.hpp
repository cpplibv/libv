// Project: libv.ui, File: src/libv/ui/property/align.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <cstdint>
#include <iosfwd>
#include <span>
#include <string_view>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

/// Enum generator version: v4
struct AlignVertical {
	using value_type = uint16_t;

public:
	static const AlignVertical& top;
	static const AlignVertical& center;
	static const AlignVertical& bottom;
	// TODO P4: libv.ui: it would be nice to have a baseline vertical alignment
	static const AlignVertical& justify;
	static const AlignVertical& justify_all;

private:
    value_type value{0}; // top

public:
    constexpr inline AlignVertical() noexcept = default;
    constexpr inline AlignVertical(const AlignVertical& other) noexcept = default;
    constexpr inline AlignVertical(AlignVertical&& other) noexcept = default;
    constexpr inline AlignVertical& operator=(const AlignVertical& other) & noexcept = default;
    constexpr inline AlignVertical& operator=(AlignVertical&& other) & noexcept = default;
    inline ~AlignVertical() noexcept = default;

    explicit constexpr inline AlignVertical(value_type value) : value{value} { }

	constexpr inline operator value_type() const noexcept {
        return value;
    }

	[[nodiscard]] constexpr inline value_type to_value() const noexcept {{
        return value;
    }}

public:
	[[nodiscard]] friend constexpr inline bool operator==(const AlignVertical& lhs, const AlignVertical& rhs) noexcept {
		return lhs.value == rhs.value;
	}
	[[nodiscard]] friend constexpr inline bool operator!=(const AlignVertical& lhs, const AlignVertical& rhs) noexcept {
		return lhs.value != rhs.value;
	}
	[[nodiscard]] friend constexpr inline bool operator<(const AlignVertical& lhs, const AlignVertical& rhs) noexcept {
		return lhs.value < rhs.value;
	}
	[[nodiscard]] friend constexpr inline bool operator<=(const AlignVertical& lhs, const AlignVertical& rhs) noexcept {
		return lhs.value <= rhs.value;
	}
	[[nodiscard]] friend constexpr inline bool operator>(const AlignVertical& lhs, const AlignVertical& rhs) noexcept {
		return lhs.value > rhs.value;
	}
	[[nodiscard]] friend constexpr inline bool operator>=(const AlignVertical& lhs, const AlignVertical& rhs) noexcept {
		return lhs.value >= rhs.value;
	}

public:
	[[nodiscard]] std::string_view to_string() const noexcept;
	friend std::ostream& operator<<(std::ostream& os, const AlignVertical& var);

public:
	[[nodiscard]] static std::span<const AlignVertical> list() noexcept;

public:
	[[nodiscard]] float to_info() const noexcept;
};

// <editor-fold defaultstate="collapsed" desc="AlignVertical constant declarations ...">

namespace detail {
static constexpr AlignVertical storage_AlignVertical[] = {
	AlignVertical{0}, // top
	AlignVertical{1}, // center
	AlignVertical{2}, // bottom
	AlignVertical{3}, // justify
	AlignVertical{4}, // justify_all
};
} // namespace detail

inline const AlignVertical& AlignVertical::top        {detail::storage_AlignVertical[0]};
inline const AlignVertical& AlignVertical::center     {detail::storage_AlignVertical[1]};
inline const AlignVertical& AlignVertical::bottom     {detail::storage_AlignVertical[2]};
inline const AlignVertical& AlignVertical::justify    {detail::storage_AlignVertical[3]};
inline const AlignVertical& AlignVertical::justify_all{detail::storage_AlignVertical[4]};

// </editor-fold>

// -------------------------------------------------------------------------------------------------

/// Enum generator version: v4
struct AlignHorizontal {
	using value_type = uint16_t;

public:
	static const AlignHorizontal& left;
	static const AlignHorizontal& center;
	static const AlignHorizontal& right;
	// TODO P5: libv.ui: it would be nice to have a baseline horizontal alignment, even if it wont be ever used, and is basically a left align, still.
	static const AlignHorizontal& justify;
	static const AlignHorizontal& justify_all;

private:
    value_type value{0}; // left

public:
    constexpr inline AlignHorizontal() noexcept = default;
    constexpr inline AlignHorizontal(const AlignHorizontal& other) noexcept = default;
    constexpr inline AlignHorizontal(AlignHorizontal&& other) noexcept = default;
    constexpr inline AlignHorizontal& operator=(const AlignHorizontal& other) & noexcept = default;
    constexpr inline AlignHorizontal& operator=(AlignHorizontal&& other) & noexcept = default;
    inline ~AlignHorizontal() noexcept = default;

    explicit constexpr inline AlignHorizontal(value_type value) : value{value} { }

	constexpr inline operator value_type() const noexcept {
        return value;
    }

	[[nodiscard]] constexpr inline value_type to_value() const noexcept {{
        return value;
    }}

public:
	[[nodiscard]] friend constexpr inline bool operator==(const AlignHorizontal& lhs, const AlignHorizontal& rhs) noexcept {
		return lhs.value == rhs.value;
	}
	[[nodiscard]] friend constexpr inline bool operator!=(const AlignHorizontal& lhs, const AlignHorizontal& rhs) noexcept {
		return lhs.value != rhs.value;
	}
	[[nodiscard]] friend constexpr inline bool operator<(const AlignHorizontal& lhs, const AlignHorizontal& rhs) noexcept {
		return lhs.value < rhs.value;
	}
	[[nodiscard]] friend constexpr inline bool operator<=(const AlignHorizontal& lhs, const AlignHorizontal& rhs) noexcept {
		return lhs.value <= rhs.value;
	}
	[[nodiscard]] friend constexpr inline bool operator>(const AlignHorizontal& lhs, const AlignHorizontal& rhs) noexcept {
		return lhs.value > rhs.value;
	}
	[[nodiscard]] friend constexpr inline bool operator>=(const AlignHorizontal& lhs, const AlignHorizontal& rhs) noexcept {
		return lhs.value >= rhs.value;
	}

public:
	[[nodiscard]] std::string_view to_string() const noexcept;
	friend std::ostream& operator<<(std::ostream& os, const AlignHorizontal& var);

public:
	[[nodiscard]] static std::span<const AlignHorizontal> list() noexcept;

public:
	[[nodiscard]] float to_info() const noexcept;
};

// <editor-fold defaultstate="collapsed" desc="AlignHorizontal constant declarations ...">

namespace detail {
static constexpr AlignHorizontal storage_AlignHorizontal[] = {
	AlignHorizontal{0}, // left
	AlignHorizontal{1}, // center
	AlignHorizontal{2}, // right
	AlignHorizontal{3}, // justify
	AlignHorizontal{4}, // justify_all
};
} // namespace detail

inline const AlignHorizontal& AlignHorizontal::left       {detail::storage_AlignHorizontal[0]};
inline const AlignHorizontal& AlignHorizontal::center     {detail::storage_AlignHorizontal[1]};
inline const AlignHorizontal& AlignHorizontal::right      {detail::storage_AlignHorizontal[2]};
inline const AlignHorizontal& AlignHorizontal::justify    {detail::storage_AlignHorizontal[3]};
inline const AlignHorizontal& AlignHorizontal::justify_all{detail::storage_AlignHorizontal[4]};

// </editor-fold>

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
